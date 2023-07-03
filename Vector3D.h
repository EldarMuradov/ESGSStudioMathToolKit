#pragma once
#include <PxPhysics.h>
#include <PxPhysicsAPI.h>
#include <cmath>
#include "Math.h"
#include "Prerequisites.h"
#include "DLL.h"

class ESGS_EXPORT Vector3D
{
public:
	Vector3D() : x(0), y(0), z(0)
	{
	}

	Vector3D(float x, float y, float z) : x(x), y(y), z(z)
	{
	}

	Vector3D(const Vector3D& vector) : x(vector.x), y(vector.y), z(vector.z)
	{
	}

	Vector3D(const physx::PxVec3& vector) : x(vector.x), y(vector.y), z(vector.z)
	{
	}

	Vector3D(const physx::PxExtendedVec3& vector) : x(vector.x), y(vector.y), z(vector.z)
	{
	}

	static Vector3D lerp(const Vector3D& start, const Vector3D& end, float delta) noexcept
	{
		Vector3D v;
		v.x = start.x * (1.0f - delta) + end.x * (delta);
		v.y = start.y * (1.0f - delta) + end.y * (delta);
		v.z = start.z * (1.0f - delta) + end.z * (delta);
		return v;
	}

	Vector3D cross(const Vector3D& rhs)
	{
		return Vector3D(
			y * rhs.z - z * rhs.y,
			z * rhs.x - x * rhs.z,
			x * rhs.y - y * rhs.x);
	}

	static Vector3D cross(const Vector3D& v1, const Vector3D& v2)
	{
		Vector3D res;
		res.x = (v1.y * v2.z) - (v1.z * v2.y);
		res.y = (v1.z * v2.x) - (v1.x * v2.z);
		res.z = (v1.x * v2.y) - (v1.y * v2.x);

		return res;
	}

	static Vector3D fromQuaternion(float x, float y, float z, float w)
	{
		Vector3D v;
		float t0 = 2.0 * (w * x + y * z);
		float t1 = 1.0 - 2.0 * (x * x + y * y);
		v.x = atan2(t0, t1) * rad2deg;

		float t2 = 2 * (y *w - z * x);
		t2 = t2 > 1.0 ? 1.0 : t2;

		v.y = asin(t2) * rad2deg;

		float t3 = 2.0 * (w * z + y * x);
		float t4 = 1.0 - 2.0 * (z * z + y * y);

		v.z = atan2(t3, t4) * rad2deg;

		return v;
	}

	float angle(Vector3D& v) 
	{
		float d = dot(v);

		return acos(d) * rad2deg;
	}

	void rotate(Vector3D& ang) 
	{
		float angleDeg = angle(ang);

		Vector3D axis = cross(*this, ang);
	}

	float magnitude()
	{ 
		return (float)sqrt(x * x + y * y + z * z);
	}

	float dot(Vector3D& vector) noexcept
	{
		return x * vector.x + y * vector.y + z * vector.z;
	}

	Vector3D getConjugate() noexcept
	{
		return Vector3D(-x, -y, -z);
	}

	void normalize()
	{
		float mag = magnitude();
		if (mag > epsilon)
		{
			x /= mag;
			y /= mag;
			z /= mag;
		}
		else
		{
			x = 0; 
			y = 0;
			z = 0;
		}
	}

	static Vector3D normalize(Vector3D value)
	{
		float mag = value.magnitude();
		if (mag > epsilon)
		{
			value.x /= mag;
			value.y /= mag;
			value.z /= mag;
		}
		else
		{
			value.x = 0;
			value.y = 0;
			value.z = 0;
		}
		return Vector3D(value.x, value.y, value.z);
	}

	static Vector3D up() noexcept
	{
		return Vector3D(0, 1, 0);
	}

	Vector3D getUp() noexcept
	{
		return Vector3D(0, y, 0);
	}

	static Vector3D down() noexcept
	{
		return Vector3D(0, -1, 0);
	}

	Vector3D getDown() noexcept
	{
		return Vector3D(0, -y, 0);
	}

	static Vector3D right() noexcept
	{
		return Vector3D(1, 0, 0);
	}

	Vector3D getRight() noexcept
	{
		return Vector3D(x, 0, 0);
	}

	static Vector3D left() noexcept
	{
		return Vector3D(-1, 0, 0);
	}

	Vector3D getLeft() noexcept
	{
		return Vector3D(-x, 0, 0);
	}

	static Vector3D forward() noexcept
	{
		return Vector3D(0, 0, 1);
	}

	Vector3D getForward() noexcept
	{
		return Vector3D(0, 0, z);
	}

	static Vector3D backward() noexcept
	{
		return Vector3D(0, 0, -1);
	}

	Vector3D getBackward() noexcept
	{
		return Vector3D(0, 0, -z);
	}

	Vector3D operator *(float num)
	{
		return Vector3D(x * num, y * num, z * num);
	}

	Vector3D operator +(Vector3D vec) const
	{
		return Vector3D(x + vec.x, y + vec.y, z + vec.z);
	}

	Vector3D operator -(Vector3D vec) const
	{
		return Vector3D(x - vec.x, y - vec.y, z - vec.z);
	}

	Vector3D operator -() const
	{
		return Vector3D(-x, -y, -z);
	}

	Vector3D operator *(Vector3D vec) const
	{
		return Vector3D(x * vec.x, y * vec.y, z * vec.z);
	}

	Vector3D operator /(Vector3D vec) const
	{
		return Vector3D(x / vec.x, y / vec.y, z / vec.z);
	}

	Vector3D operator +(const physx::PxVec3& vec)
	{
		return Vector3D(x + vec.x, y + vec.y, z + vec.z);
	}

	Vector3D operator -(const physx::PxVec3& vec)
	{
		return Vector3D(x - vec.x, y - vec.y, z - vec.z);
	}

	bool operator !=(const Vector3D& vec)
	{
		return (x != vec.x) && (y != vec.y) && (z != vec.z);
	}

	bool operator ==(const Vector3D& vec)
	{
		return (x == vec.x) && (y == vec.y) && (z == vec.z);
	}

	~Vector3D()
	{
	}

public:
	float x = 0;
	float y = 0;
	float z = 0;
};