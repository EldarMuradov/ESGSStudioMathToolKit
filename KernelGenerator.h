#pragma once
#include <random>
#include <DirectXMath.h>
#include <d3d11.h>

#ifndef SSAO_KERNEL_SIZE
#define SSAO_KERNEL_SIZE 64
#endif // !SSAO_KERNEL_SIZE

#ifndef SSAO_OFFSET_SIZE
#define SSAO_OFFSET_SIZE 14
#endif // !SSAO_KERNEL_SIZE

class KernelGenerator
{
public:
	KernelGenerator();
	~KernelGenerator();

	void generateSSAOKernel();
	void generateOffsetVectors();

	DirectX::XMFLOAT3* getSSAOKernel();
	DirectX::XMFLOAT4* getSSAOOffset();

private:
	std::default_random_engine generator;


	DirectX::XMFLOAT3 m_ssao_kernel[SSAO_KERNEL_SIZE];


	DirectX::XMFLOAT4 m_ssao_offset[SSAO_OFFSET_SIZE];
};