#pragma once
#include <cfloat>
#include <cmath>


class Vector
{
public:
	float x, y, z;

	Vector();
	Vector(const Vector&& left) noexcept;
	Vector(const Vector& left);
	Vector(Vector& left);
	Vector(float _x, float _y, float _z);
	Vector(float* ptr);
	Vector(float val);
	
	void init(float _x, float _y, float _z);

	void makeInf();
	void makeZero();

	void normalize();
	void clamp();
	
	float length() const;
	float length2d() const;
	float distance_to(const Vector& to) const;
	
	[[nodiscard]] bool is_valid() const;

	[[nodiscard]] float* base();
	[[nodiscard]] float const* base() const;

	float operator[] (int idx) const;
	float& operator[] (int idx);

	Vector& operator=(const Vector& left);
	bool operator==(const Vector& other) const;

	Vector operator+(const Vector& left) const;
	Vector operator-(const Vector& left) const;
	Vector operator*(const Vector& left) const;
	Vector operator/(const Vector& left) const;

	Vector& operator+=(const Vector& left);
	Vector& operator-=(const Vector& left);
	Vector& operator*=(const Vector& left);
	Vector& operator/=(const Vector& left);

	Vector toStringFriendly() const
	{
		if (!is_valid())
			return { 0.f };

		auto out = *this;
		
		if (out.x > FLT_MAX)
			out.x = FLT_MAX;
		if (out.y > FLT_MAX)
			out.y = FLT_MAX;
		if (out.z > FLT_MAX)
			out.z = FLT_MAX;

		out.x = roundf(out.x);
		out.y = roundf(out.y);
		out.z = roundf(out.z);
		
		return out;
	}
};

class __declspec(align(16)) CVectorAligned : public Vector {
public:
	inline CVectorAligned(void) {};
	inline CVectorAligned(float X, float Y, float Z) {
		init(X, Y, Z);
	}

public:
	explicit CVectorAligned(const Vector& vOther) {
		init(vOther.x, vOther.y, vOther.z);
	}

	CVectorAligned& operator=(const Vector& vOther) {
		init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	float w;
};