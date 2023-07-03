#pragma once
#include <PxPhysics.h>
#include <PxPhysicsAPI.h>
#include "DLL.h"

class ESGS_EXPORT Vector2D
{
public:
	Vector2D() : x(0), y(0)
	{
	}

	Vector2D(float x, float y) : x(x), y(y)
	{
	}

	Vector2D(const Vector2D& vector) : x(vector.x), y(vector.y)
	{
	}

	Vector2D(const physx::PxVec2& vector) : x(vector.x), y(vector.y)
	{
	}

	Vector2D operator *(float num)
	{
		return Vector2D(x * num, y * num);
	}

	Vector2D operator +(Vector2D vec) const
	{
		return Vector2D(x + vec.x, y + vec.y);
	}

	Vector2D operator -(Vector2D vec) const
	{
		return Vector2D(x - vec.x, y - vec.y);
	}

	Vector2D operator *(const Vector2D& vec)
	{
		return Vector2D(x * vec.x, y * vec.y);
	}

	Vector2D operator +(const physx::PxVec2& vec)
	{
		return Vector2D(x + vec.x, y + vec.y);
	}

	bool operator !=(const Vector2D& vec)
	{
		return (x != vec.x) && (y != vec.y);
	}

	bool operator ==(const Vector2D& vec)
	{
		return (x == vec.x) && (y == vec.y);
	}

	~Vector2D()
	{
	}

public:
	float x = 0;
	float y = 0;
};