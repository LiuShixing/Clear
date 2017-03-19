#pragma once

#include"..\Common\Effect.h"

class BasicEffect:public Effect
{
public:
	static int Init(ID3D11Device* d3dDevice,const std::wstring& filename);
public:
	static ID3DX11EffectTechnique*                  mTech;
	static ID3DX11EffectTechnique*                  mTechAlphaClip;

	static ID3DX11EffectMatrixVariable*             mWorld;
	static ID3DX11EffectMatrixVariable*             mWorldInvTranspose;
	static ID3DX11EffectMatrixVariable*             mWorldViewProj;
	static ID3DX11EffectMatrixVariable*             mTexTransform;

	static ID3DX11EffectVectorVariable*             mEyePosW;

	static ID3DX11EffectVariable*                  mDirLight;
	static ID3DX11EffectVariable*                  mMaterial;

	static ID3DX11EffectShaderResourceVariable*      mDiffuseMap;
};

class NoColorEffect :Effect
{
public:
	static int Init(ID3D11Device* d3dDevice, const std::wstring& filename);
public:
	static ID3DX11EffectTechnique*                  mTech;

	static ID3DX11EffectMatrixVariable*             mWorld;
	static ID3DX11EffectMatrixVariable*             mWorldInvTranspose;
	static ID3DX11EffectMatrixVariable*             mWorldViewProj;
};