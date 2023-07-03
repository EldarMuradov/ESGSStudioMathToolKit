#include "KernelGenerator.h"
#include "Utilities.h"
#include <d3d11.h>
#include "GraphicsEngine.h"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace DirectX;

std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);

KernelGenerator::KernelGenerator()
{
}

KernelGenerator::~KernelGenerator()
{
}

void KernelGenerator::generateSSAOKernel()
{
    DirectX::XMFLOAT3 tmpSample;
    generator.seed(std::random_device()());

    std::vector<DirectX::PackedVector::XMCOLOR> randomVectors(256 * 256);

    for (int i = 0; i < SSAO_KERNEL_SIZE; i++)
    {
        tmpSample.x = randomFloats(generator) * 2.0f - 1.0f;
        tmpSample.y = randomFloats(generator) * 2.0f - 1.0f;
        tmpSample.z = randomFloats(generator);

        DirectX::XMVECTOR tmpVector;
        tmpVector.m128_f32[0] = tmpSample.x;
        tmpVector.m128_f32[1] = tmpSample.y;
        tmpVector.m128_f32[2] = tmpSample.z;

        tmpVector = DirectX::XMVector3Normalize(tmpVector);

        tmpSample.x = tmpVector.m128_f32[0];
        tmpSample.y = tmpVector.m128_f32[1];
        tmpSample.z = tmpVector.m128_f32[2];

        float scale = float(i) / 64.0f;
        scale = lerp(0.1f, 1.0f, scale * scale);
        tmpSample.x *= scale;
        tmpSample.y *= scale;
        tmpSample.z *= scale;

        m_ssao_kernel[i] = tmpSample;
    }
}

void KernelGenerator::generateOffsetVectors()
{
    m_ssao_offset[0] = DirectX::XMFLOAT4(+1.0f, +1.0f, +1.0f, 0.0f);
    m_ssao_offset[1] = DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 0.0f);

    m_ssao_offset[2] = DirectX::XMFLOAT4(-1.0f, +1.0f, +1.0f, 0.0f);
    m_ssao_offset[3] = DirectX::XMFLOAT4(+1.0f, -1.0f, -1.0f, 0.0f);

    m_ssao_offset[4] = DirectX::XMFLOAT4(+1.0f, +1.0f, -1.0f, 0.0f);
    m_ssao_offset[5] = DirectX::XMFLOAT4(-1.0f, -1.0f, +1.0f, 0.0f);

    m_ssao_offset[6] = DirectX::XMFLOAT4(-1.0f, +1.0f, -1.0f, 0.0f);
    m_ssao_offset[7] = DirectX::XMFLOAT4(+1.0f, -1.0f, +1.0f, 0.0f);

    // 6 face center point vectors
    m_ssao_offset[8] = DirectX::XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f);
    m_ssao_offset[9] = DirectX::XMFLOAT4(+1.0f, 0.0f, 0.0f, 0.0f);

    m_ssao_offset[10] = DirectX::XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
    m_ssao_offset[11] = DirectX::XMFLOAT4(0.0f, +1.0f, 0.0f, 0.0f);

    m_ssao_offset[12] = DirectX::XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
    m_ssao_offset[13] = DirectX::XMFLOAT4(0.0f, 0.0f, +1.0f, 0.0f);


    std::default_random_engine generator;
    generator.seed(std::random_device()());
    std::uniform_real_distribution<float> randomFloatsOffset(0.25f, 1.0f);

    for (int i = 0; i < 14; ++i)
    {
        float s = randomFloatsOffset(generator);

        DirectX::XMVECTOR v = s * DirectX::XMVector4Normalize(DirectX::XMLoadFloat4(&m_ssao_offset[i]));

        DirectX::XMStoreFloat4(&m_ssao_offset[i], v);
    }
}

DirectX::XMFLOAT3* KernelGenerator::getSSAOKernel()
{
    return m_ssao_kernel;
}

DirectX::XMFLOAT4* KernelGenerator::getSSAOOffset()
{
    return m_ssao_offset;
}