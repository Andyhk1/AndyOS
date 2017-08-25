#pragma once
#include "../GL/GL.h"

struct Vector
{
public:
	float x;
	float y;
	float z;

	Vector();
	Vector(float x, float y, float z);

	Vector EulerAngles();
	gl::Vector4 ToVector4();

	Vector operator +(const Vector& b)
	{
		return Vector(x + b.x, y + b.y, z + b.z);
	}

	Vector& operator +=(const Vector& b)
	{
		this->x += b.x;
		this->y += b.y;
		this->z += b.z;
		return *this;
	}

	Vector operator -(const Vector& b)
	{
		return Vector(x - b.x, y - b.y, z - b.z);
	}

	Vector& operator -=(const Vector& b)
	{
		this->x -= b.x;
		this->y -= b.y;
		this->z -= b.z;
		return *this;
	}

	Vector operator -()
	{
		return Vector(-x, -y, -z);
	}

	Vector operator *(float f)
	{
		return Vector(x * f, y * f, z * f);
	}

	Vector operator /(float f)
	{
		return Vector(x / f, y / f, z / f);
	}

	float Magnitude()
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vector Normalized()
	{
		return *this / Magnitude();
	}

	float DotProduct(const Vector& v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	Vector CrossProduct(const Vector& v)
	{
		return Vector(y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x);
	}
};