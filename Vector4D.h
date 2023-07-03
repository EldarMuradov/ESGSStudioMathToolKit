#pragma once
#include "Vector3D.h"
#include <PxPhysics.h>
#include <PxPhysicsAPI.h>
#include "Prerequisites.h"
#include "DLL.h"

class ESGS_EXPORT Vector4D
{
public:
	Vector4D() : x(0), y(0), z(0), w(0)
	{
	}

	Vector4D(float x, float y, float z,float w) : x(x), y(y), z(z), w(w)
	{
	}

	Vector4D(const Vector4D& vector) : x(vector.x), y(vector.y), z(vector.z), w(vector.w)
	{
	}

	Vector4D(const physx::PxVec4& vector) : x(vector.x), y(vector.y), z(vector.z), w(vector.w)
	{
	}

	Vector4D(const Vector3D& vector) : x(vector.x), y(vector.y), z(vector.z), w(1.0)
	{
	}

	Vector4D(const physx::PxVec3& vector) : x(vector.x), y(vector.y), z(vector.z), w(1.0)
	{
	}

	void cross(Vector4D& v1, Vector4D& v2, Vector4D& v3)
	{
		this->x = v1.y * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.y * v3.w - v3.y * v2.w) + v1.w * (v2.y * v3.z - v2.z * v3.y);
		this->y = -(v1.x * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.z - v3.x * v2.z));
		this->z = v1.x * (v2.y * v3.w - v3.y * v2.w) - v1.y * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.y - v3.x * v2.y);
		this->w = -(v1.x * (v2.y * v3.z - v3.y * v2.z) - v1.y * (v2.x * v3.z - v3.x * v2.z) + v1.z * (v2.x * v3.y - v3.x * v2.y));
	}

	bool operator !=(const Vector4D& vec)
	{
		return (x != vec.x) && (y != vec.y) && (z != vec.z) && (w != vec.w);
	}

	bool operator ==(const Vector4D& vec)
	{
		return (x == vec.x) && (y == vec.y) && (z == vec.z) && (w == vec.w);
	}
	
	~Vector4D()
	{
	}

public:
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;
};