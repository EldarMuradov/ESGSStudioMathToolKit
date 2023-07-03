#pragma once
#include "Vector3D.h"
#include <cmath>
#include "Matrix4x4.h"
#include "Math.h"
#include "DLL.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

class ESGS_EXPORT Quaternion
{
public:
	Quaternion(float x, float y, float z, float w) 
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Quaternion()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 1;
	}

	void set(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	void set(Quaternion q)
	{
		this->x = q.x;
		this->y = q.y;
		this->z = q.z;
		this->w = q.w;
	}

	static Quaternion identity() 
	{
		return Quaternion(0, 0, 0, 1);
	}

	Quaternion operator *(Quaternion rhs)
	{
		return Quaternion(
			w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
			w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
			w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x,
			w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);
	}

	Quaternion operator *(float num)
	{
		return Quaternion(x * num, y * num, z * num, w * num);
	}

	Quaternion operator /(float num)
	{
		return Quaternion(x / num, y / num, z / num, w / num);
	}

	Vector3D operator *(Vector3D point)
	{
		float x = this->x * 2;
		float y = this->y * 2;
		float z = this->z * 2;
		float xx = x * x;
		float yy = y * y;
		float zz = z * z;
		float xy = x * y;
		float xz = x * z;
		float yz = y * z;
		float wx = w * x;
		float wy = w * y;
		float wz = w * z;

		Vector3D res;
		res.x = (1 - (yy + zz)) * point.x + (xy - wz) * point.y + (xz + wy) * point.z;
		res.y = (xy + wz) * point.x + (1 - (xx + zz)) * point.y + (yz - wx) * point.z;
		res.z = (xz - wy) * point.x + (yz + wx) * point.y + (1 - (xx + yy)) * point.z;
		return res;
	}

	bool isEqualUsingDot(float dot)
	{
		// Returns false in the presence of NaN values.
		return dot > 1.0f - epsilon;
	}

	bool operator ==(const Quaternion& rhs)
	{
		return isEqualUsingDot(dot(rhs));
	}

	bool operator !=(const Quaternion& rhs)
	{
		// Returns true in the presence of NaN values.
		return !(*this == rhs);
	}

	float dot(const Quaternion& a)
	{
		return a.x * x + a.y * y + a.z * z + a.w * w;
	}

	float angle(const Quaternion& a)
	{
		float d = MIN(abs(dot(a)), 1.0f);
		return isEqualUsingDot(d) ? 0.0f : acos(d) * 2.0f * rad2deg;
	}

	static Vector3D internalMakePositive(Vector3D euler)
	{
		float negativeFlip = -0.0001f * 57.29578f;
		float positiveFlip = 360.0f + negativeFlip;

		if (euler.x < negativeFlip)
			euler.x += 360.0f;
		else if (euler.x > positiveFlip)
			euler.x -= 360.0f;

		if (euler.y < negativeFlip)
			euler.y += 360.0f;
		else if (euler.y > positiveFlip)
			euler.y -= 360.0f;

		if (euler.z < negativeFlip)
			euler.z += 360.0f;
		else if (euler.z > positiveFlip)
			euler.z -= 360.0f;

		return euler;
	}

	Quaternion rotateTowards(const Quaternion& to, float maxDegreesDelta = 360)
	{
		float ang = angle(to);
		if (ang == 0.0f) 
			return to;
		return slerp(to, MIN(1.0f, maxDegreesDelta / ang));
	}

	Quaternion getConjugate()
	{
		return Quaternion(-x, -y, -z, w);
	}

	void setLookRotation(Vector3D view)
	{
		Vector3D up = Vector3D(0, 1, 0);
		setLookRotation(view, up);
	}

	void setLookRotation(Vector3D view, Vector3D up)
	{
		Quaternion q = lookRotation(view, up);
		set(q);
	}

	Quaternion slerp(Quaternion q, float t)
	{
		Quaternion ret;

		float fCos = dot(q);

		if ((1.0f + fCos) > epsilon)
		{
			float fCoeff0, fCoeff1;

			if ((1.0f - fCos) > epsilon)
			{
				float omega = acos(fCos);
				float invSin = 1.0f / sin(omega);
				fCoeff0 = sin((1.0f - t) * omega) * invSin;
				fCoeff1 = sin(t * omega) * invSin;
			}
			else
			{
				fCoeff0 = 1.0f - t;
				fCoeff1 = t;
			}

			ret.x = fCoeff0 * x + fCoeff1 * q.x;
			ret.y = fCoeff0 * y + fCoeff1 * q.y;
			ret.z = fCoeff0 * z + fCoeff1 * q.z;
			ret.w = fCoeff0 * w + fCoeff1 * q.w;
		}
		else
		{
			float fCoeff0 = sin((1.0f - t) * pi * 0.5f);
			float fCoeff1 = sin(t * pi * 0.5f);

			ret.x = fCoeff0 * x - fCoeff1 * y;
			ret.y = fCoeff0 * y + fCoeff1 * x;
			ret.z = fCoeff0 * z - fCoeff1 * w;
			ret.w = z;
		}

		return ret;
	}

	Quaternion lookRotation(Vector3D forward, Vector3D up)
	{
		forward.normalize();

		Vector3D vector = forward.normalize(forward);
		Vector3D vector2 = up.normalize(up.cross(vector));
		Vector3D vector3 = vector.cross(vector2);
		float m00 = vector2.x;
		float m01 = vector2.y;
		float m02 = vector2.z;
		float m10 = vector3.x;
		float m11 = vector3.y;
		float m12 = vector3.z;
		float m20 = vector.x;
		float m21 = vector.y;
		float m22 = vector.z;

		float num8 = (m00 + m11) + m22;
		Quaternion quaternion = Quaternion();
		if (num8 > 0)
		{
			float num = (float)sqrt(num8 + 1);
			quaternion.w = num * 0.5f;
			num = 0.5f / num;
			quaternion.x = (m12 - m21) * num;
			quaternion.y = (m20 - m02) * num;
			quaternion.z = (m01 - m10) * num;
			return quaternion;
		}
		if ((m00 >= m11) && (m00 >= m22))
		{
			float num7 = (float)sqrt(((1 + m00) - m11) - m22);
			float num4 = 0.5f / num7;
			quaternion.x = 0.5f * num7;
			quaternion.y = (m01 + m10) * num4;
			quaternion.z = (m02 + m20) * num4;
			quaternion.w = (m12 - m21) * num4;
			return quaternion;
		}
		if (m11 > m22)
		{
			float num6 = (float)sqrt(((1 + m11) - m00) - m22);
			float num3 = 0.5f / num6;
			quaternion.x = (m10 + m01) * num3;
			quaternion.y = 0.5f * num6;
			quaternion.z = (m21 + m12) * num3;
			quaternion.w = (m20 - m02) * num3;
			return quaternion;
		}
		float num5 = (float)sqrt(((1 + m22) - m00) - m11);
		float num2 = 0.5f / num5;
		quaternion.x = (m20 + m02) * num2;
		quaternion.y = (m21 + m12) * num2;
		quaternion.z = 0.5f * num5;
		quaternion.w = (m01 - m10) * num2;
		return quaternion;
	}

	static Quaternion eulerToQuaternion(Vector3D someEulerAngles)
	{
		float cX = (cos(someEulerAngles.x / 2.0f));
		float sX = (sin(someEulerAngles.x / 2.0f));

		float cY = (cos(someEulerAngles.y / 2.0f));
		float sY = (sin(someEulerAngles.y / 2.0f));

		float cZ = (cos(someEulerAngles.z / 2.0f));
		float sZ = (sin(someEulerAngles.z / 2.0f));

		Quaternion qX = Quaternion(sX, 0.0f, 0.0f, cX);
		Quaternion qY = Quaternion(0.0f, sY, 0.0f, cY);
		Quaternion qZ = Quaternion(0.0f, 0.0f, sZ, cZ);

		Quaternion q = (qY * qX) * qZ;
		return q;
	}

	static Quaternion euler(Vector3D euler) 
	{ 
		return fromEulerRad(euler * deg2rad);
	}

	Quaternion normalizeSafe(Quaternion q)
	{
		float mag = magnitude(q);
		if (mag < epsilon)
			return Quaternion::identity();
		else
			return q / mag;
	}

	float magnitude(Quaternion q)
	{
		return sqrt(sqrMagnitude(q));
	}

	float sqrMagnitude(Quaternion q)
	{
		return q.dot(q);
	}

	static Quaternion euler(float x, float y, float z)
	{
		return fromEulerRad(Vector3D(x, y, z) * deg2rad);
	}

	static Quaternion fromEulerRad(Vector3D euler) 
	{
		return eulerToQuaternion(euler);
	}

	Vector3D quaternionToEuler()
	{
		float sqw = w * w;
		float sqx = x * x;
		float sqy = y * y;
		float sqz = z * z;
		float unit = sqx + sqy + sqz + sqw;
		float test = x * w - y * z;
		Vector3D v;

		if (test > 0.4995f * unit) 
		{ 
			// singularity at north pole
			v.y = 2 * atan2(y, x);
			v.x = pi / 2;
			v.z = 0;
			return normalizeAngles(v * rad2deg);
		}
		if (test < -0.4995f * unit) 
		{ 
			// singularity at south pole
			v.y = -2 * atan2(y, x);
			v.x = -pi / 2;
			v.z = 0;
			return normalizeAngles(v * rad2deg);
		}
		Quaternion q = Quaternion(w, z, x, y);
		v.y = (float)atan2(2 * q.x * q.w + 2 * q.y * q.z, 1 - 2 * (q.z * q.z + q.w * q.w));  // Yaw
		v.x = (float)asin(2 * (q.x * q.z - q.w * q.y));  // Pitch
		v.z = (float)atan2(2 * q.x * q.y + 2 * q.z * q.w, 1 - 2 * (q.y * q.y + q.z * q.z));  // Roll
		return normalizeAngles(v * deg2rad);
	}

	static Vector3D normalizeAngles(Vector3D angles)
	{
		angles.x = normalizeAngle(angles.x * rad2deg);
		angles.y = normalizeAngle(angles.y * rad2deg);
		angles.z = normalizeAngle(angles.z * rad2deg);

		return angles;
	}

	static float normalizeAngle(float angle)
	{
		while (angle > 360)
			angle -= 360;
		while (angle < 0)
			angle += 360;
		return angle;
	}

	//Don't update unless you don't know quaternial math
	float x;
	//Don't update unless you don't know quaternial math
	float y;
	//Don't update unless you don't know quaternial math
	float z;
	//Don't update unless you don't know quaternial math
	float w;

private:
	static Quaternion euler_native(float x, float y, float z)
	{
		return fromEulerRad(Vector3D(x, y, z) * deg2rad);
	}

	friend class CsGame;
};