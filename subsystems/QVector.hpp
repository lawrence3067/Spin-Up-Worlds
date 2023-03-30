#include "main.h"

using namespace okapi;

// okapi has a very nice system of units
// you can do stuff like QLength x = 2_in; and convert it into whatever unit you want
// ex. for angles, QAngle a = 180_deg can be converted into radians by doing a.convert(radian) (returning a double)

// QVector is basically representing a vector with units attached
// ex. for length vectors, velocity vectors
// usually I use length vectors (QVector<QLength>) to represent the position of various things, like the robot or path points

// this will be used extensively for odometry
template <typename Unit> // if you don't know what templates are, look it up
struct QVector {
	Unit x, y;
	QVector() {
		x = Unit(); // initialize x and y to the default value of the unit (usually 0)
		y = Unit();
	};
	QVector(Unit _x, Unit _y) {
		x = _x;
		y = _y;
	}; // construct QVector in component form
	QVector(Unit norm, QAngle arg) {
		x = norm * cos(arg.convert(radian));
		y = norm * sin(arg.convert(radian));
	}; // construct QVector in polar form
	explicit QVector(QAngle arg) {
		x = Unit(1.0) * cos(arg.convert(radian));
		y = Unit(1.0) * sin(arg.convert(radian));
	}; // construct unit vector in polar form
	Unit norm() {
		return Unit(std::hypot(x.getValue(), y.getValue()));
	}; // find magnitude of vector (hypot(x, y) basically gives you sqrt(x^2 + y^2))
	QAngle arg() {
		double rv = std::atan2(y.getValue(), x.getValue());
		return rv * radian;
	}; // find direction that vector is pointing in
};

// these are operator overloads. this means I can add vectors just by saying vector1 + vector2, instead of like vector1.add(vector2) for instance
// the templates here get a little complicated, but just understand these are basic vector operations
template <typename U>
QVector<U> operator +(const QVector<U> &v1, const QVector<U> &v2) {
	QVector<U> rv(v1.x + v2.x, v1.y + v2.y);
	return rv;
} // QVector add

template <typename U>
QVector<U> operator -(const QVector<U> &v1, const QVector<U> &v2) {
	QVector<U> rv(v1.x - v2.x, v1.y - v2.y);
	return rv;
} // QVector sub

template <typename U>
QVector<U> &operator +=(QVector<U> &v1, const QVector<U> &v2) {
	v1.x += v2.x;
	v1.y += v2.y;
	return v1;
} // QVector inc

template <typename U>
QVector<U> &operator -=(QVector<U> &v1, const QVector<U> &v2) {
	v1.x -= v2.x;
	v1.y -= v2.y;
	return v1;
} // QVector dec

// auto is implied type, so the compiler will figure out the right return type for me
template <typename U, typename T>
auto operator *(const QVector<U> &v1, const T &s) {
	auto rvx = v1.x * s;
	auto rvy = v1.y * s;
	QVector<decltype(rvx)> rv(rvx, rvy); // decltype makes the compiler get the type of a variable
	return rv;
} // scale vector

template <typename U, typename T>
auto operator *(const U &s, const QVector<T> &v1) {
	auto rvx = v1.x * s;
	auto rvy = v1.y * s;
	QVector<decltype(rvx)> rv(rvx, rvy);
	return rv;
} // scale vector

template <typename U, typename T>
auto operator /(const QVector<U> &v1, const T &s) {
	auto rvx = v1.x / s;
	auto rvy = v1.y / s;
	QVector<decltype(rvx)> rv(rvx, rvy);
	return rv;
} // scale vector

template <typename U, typename T>
auto operator *(const QVector<U> &v1, const QVector<T> &v2) {
	return v1.x * v2.x + v1.y * v2.y;
} // QVector dot product

template <typename U, typename T>
auto cross(const QVector<U> &v1, const QVector<T> &v2) {
	return v1.x * v2.y - v1.y * v2.x;
} // QVector magnitude of cross product (cross product is actually three dimensional, but we just return the magnitude of the resulting vector)

// RQuantity is kinda like the father of all unit types. Here, I'm just returning whether the value given is negative, positive or 0, regardless of unit
// this is important specifically for cross product stuff.
template <typename M, typename L, typename T, typename A>
double sgn(RQuantity<M, L, T, A> r) {
	if(r.getValue() == 0) {
		return 0;
	} else if(r.getValue() > 0) {
		return 1.0;
	} else {
		return -1.0;
	}
}
