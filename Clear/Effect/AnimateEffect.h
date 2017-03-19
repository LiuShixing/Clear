#pragma once

#include"..\Common\Effect.h"

class AnimateEffect:public Effect
{
public:
	static int Init(ID3D11Device* d3dDevice,const std::wstring& filename);

	static void SetBoneTransforms(XMFLOAT4X4* M, int cnt){ mBoneTransforms->SetMatrixArray(reinterpret_cast<float*>(M), 0, cnt); }
public:
	static ID3DX11EffectTechnique*                  mTech;
	static ID3DX11EffectTechnique*                  mTechAlphaClip;

	static ID3DX11EffectMatrixVariable*             mWorld;
	static ID3DX11EffectMatrixVariable*             mWorldInvTranspose;
	static ID3DX11EffectMatrixVariable*             mWorldViewProj;
	static ID3DX11EffectMatrixVariable*             mTexTransform;
	static ID3DX11EffectMatrixVariable*             mBoneTransforms;

	static ID3DX11EffectVectorVariable*             mEyePosW;

	static ID3DX11EffectVariable*                  mDirLight;
	static ID3DX11EffectVariable*                  mMaterial;

	static ID3DX11EffectShaderResourceVariable*      mDiffuseMap;
};