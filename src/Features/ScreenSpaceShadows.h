#pragma once

#include "Buffer.h"

class ScreenSpaceShadows
{
public:
	static ScreenSpaceShadows* GetSingleton()
	{
		static ScreenSpaceShadows singleton;
		return &singleton;
	}

	bool enabledFeature = false;
	std::string version;

	struct Settings
	{
		uint32_t MaxSamples = 24;
		float FarDistanceScale = 0.025f;
		float FarThicknessScale = 0.025f;
		float FarHardness = 8.0f;
		float NearDistance = 8.0f;
		float NearThickness = 1.0f;
		float NearHardness = 32.0f;
		float BlurRadius = 0.5f;
		float BlurDropoff = 0.005f;
	};

	struct alignas(16) PerPass
	{
		uint32_t EnableSSS;
		uint32_t FrameCount;
		uint32_t pad[2];
	};

	struct alignas(16) RaymarchCB
	{
		DirectX::XMFLOAT2 BufferDim;
		DirectX::XMFLOAT2 RcpBufferDim;
		DirectX::XMMATRIX ProjMatrix;
		DirectX::XMMATRIX InvProjMatrix;
		DirectX::XMVECTOR InvDirLightDirectionVS;
		float ShadowDistance = 10000;
		Settings Settings;
	};

	Settings settings;

	ConstantBuffer* perPass = nullptr;

	bool enabled = true;

	ID3D11SamplerState* computeSampler = nullptr;

	Texture2D* screenSpaceShadowsTexture = nullptr;
	Texture2D* screenSpaceShadowsTextureTemp = nullptr;

	ConstantBuffer* raymarchCB = nullptr;
	ID3D11ComputeShader* raymarchProgram = nullptr;

	ID3D11ComputeShader* horizontalBlurProgram = nullptr;
	ID3D11ComputeShader* verticalBlurProgram = nullptr;
	
	bool renderedScreenCamera = false;

	void SetupResources();

	void DrawSettings();
	void ModifyGrass(const RE::BSShader* shader, const uint32_t descriptor);
	void ModifyDistantTree(const RE::BSShader*, const uint32_t descriptor);

	void ClearComputeShader();
	ID3D11ComputeShader* GetComputeShader();
	ID3D11ComputeShader* GetComputeShaderHorizontalBlur();
	ID3D11ComputeShader* GetComputeShaderVerticalBlur();

	void ModifyLighting(const RE::BSShader* shader, const uint32_t descriptor);
	void Draw(const RE::BSShader* shader, const uint32_t descriptor);

	void Load(json& o_json);
	void Save(json& o_json);

	bool ValidateCache(CSimpleIniA& a_ini);
	void WriteDiskCacheInfo(CSimpleIniA& a_ini);
	void Reset();
};
