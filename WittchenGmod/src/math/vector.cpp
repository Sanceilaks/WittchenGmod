#include "vector.h"
#include <limits>
#include <cmath>
#include <limits>
#include <algorithm>

Vector::Vector()
{
	makeInf();
}

Vector::Vector(const Vector&& left) noexcept : x(left.x), y(left.y), z(left.z)
{
	
}

Vector::Vector(const Vector& left) : x(left.x), y(left.y), z(left.z)
{
	
}

Vector::Vector(Vector& left) : x(left.x), y(left.y), z(left.z)
{
}

Vector::Vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
{
}

Vector::Vector(float* ptr) : x(ptr[0]), y(ptr[1]), z(ptr[2])
{
}

Vector::Vector(float val) : x(val), y(val), z(val)
{
	
}

void Vector::init(float _x, float _y, float _z)
{
	x = _x; y = _y; z = _z;
}

void Vector::makeInf()
{
	x = y = z = std::numeric_limits<float>::infinity();
}

void Vector::makeZero()
{
	x = y = z = 0.f;
}

void Vector::normalize()
{
	while (x > 89.0f)
		x -= 180.0f;
	while (x < -89.0f)
		x += 180.0f;
	while (y < -180.0f)
		y += 360.0f;
	while (y > 180.0f)
		y -= 360.0f;
	z = 0.f;
}

void Vector::clamp()
{
	while (this->x < -180.0f)
		this->x += 360.0f;
	while (this->x > 180.0f)
		this->x -= 360.0f;
	if (this->x > 89.0f)
		this->x = 89.0f;
	if (this->x < -89.0f)
		this->x = -89.0f;
	while (this->y < -180.0f)
		this->y += 360.0f;
	while (this->y > 180.0f)
		this->y -= 360.0f;

	this->z = 0.0f;
}

float Vector::length() const
{
	return sqrt(x * x + y * y + z * z);
}

float Vector::length2d() const
{
	return sqrt(x * x + y * y);
}

float Vector::distance_to(const Vector& to) const
{
	return (*this - to).length();
}

bool Vector::is_valid() const
{
	return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
}

float* Vector::base()
{
	return reinterpret_cast<float*>(this);
}

float const* Vector::base() const
{
	return reinterpret_cast<float const*>(this);
}

float Vector::operator[](int idx) const
{
	return ((float*)this)[idx];
}

float& Vector::operator[](int idx)
{
	return ((float*)this)[idx];
}

Vector& Vector::operator=(const Vector& left)
= default;

bool Vector::operator==(const Vector& other) const
{
	return x == other.x && y == other.y && z == other.z;
}

Vector Vector::operator+(const Vector& left) const
{
	return { x + left.x, y + left.y, z + left.z };
}

Vector Vector::operator-(const Vector& left) const
{
	return { x - left.x, y - left.y, z - left.z };
}

Vector Vector::operator*(const Vector& left) const
{
	return { x * left.x, y * left.y, z * left.z };
}

Vector Vector::operator/(const Vector& left) const
{
	return { x / left.x, y / left.y, z / left.z };
}


Vector& Vector::operator+=(const Vector& left)
{
	x += left.x;
	y += left.y;
	z += left.z;
	return *this;
}

Vector& Vector::operator-=(const Vector& left)
{
	x -= left.x;
	y -= left.y;
	z -= left.z;
	return *this;
}

Vector& Vector::operator*=(const Vector& left)
{
	x *= left.x;
	y *= left.y;
	z *= left.z;
	return *this;
}

Vector& Vector::operator/=(const Vector& left)
{
	x /= left.x;
	y /= left.y;
	z /= left.z;
	return *this;
}
