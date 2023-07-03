#pragma once
#include "Vector4D.h"
#include "Matrix4x4.h"

struct WorldToScreenPoint 
{
    WorldToScreenPoint()
    {
    
    }

    ~WorldToScreenPoint() 
    {
    
    }

	static Vector4D multipleOverMatrix4x4(Matrix4x4 matrix, Vector4D vector)
    {
        float x, y, z, w;
        x = vector.x * matrix.m_mat[0][0] +
            vector.y * matrix.m_mat[1][0] +
            vector.z * matrix.m_mat[2][0] +
            vector.w * matrix.m_mat[3][0];
        y = vector.x * matrix.m_mat[0][1] +
            vector.y * matrix.m_mat[1][1] +
            vector.z * matrix.m_mat[2][1] +
            vector.w * matrix.m_mat[3][1];
        z = vector.x * matrix.m_mat[0][2] +
            vector.y * matrix.m_mat[1][2] +
            vector.z * matrix.m_mat[2][2] +
            vector.w * matrix.m_mat[3][2];
        w = vector.x * matrix.m_mat[0][3] +
            vector.y * matrix.m_mat[1][3] +
            vector.z * matrix.m_mat[2][3] +
            vector.w * matrix.m_mat[3][3];
        return Vector4D(x, y, z, w);
    }
};