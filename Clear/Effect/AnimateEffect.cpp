#include"AnimateEffect.h"


ID3DX11EffectTechnique*                  AnimateEffect::mTech = NULL; 
ID3DX11EffectTechnique*                  AnimateEffect::mTechAlphaClip = NULL;
ID3DX11EffectMatrixVariable*             AnimateEffect::mWorld=NULL;
ID3DX11EffectMatrixVariable*             AnimateEffect::mWorldInvTranspose=NULL;
ID3DX11EffectMatrixVariable*             AnimateEffect::mWorldViewProj=NULL;
ID3DX11EffectMatrixVariable*             AnimateEffect::mTexTransform=NULL;
ID3DX11EffectMatrixVariable*             AnimateEffect::mBoneTransforms=NULL;
ID3DX11EffectVectorVariable*             AnimateEffect::mEyePosW=NULL;
ID3DX11EffectVariable*                   AnimateEffect::mDirLight=NULL;
ID3DX11EffectVariable*                   AnimateEffect::mMaterial=NULL;
ID3DX11EffectShaderResourceVariable*      AnimateEffect::mDiffuseMap=NULL;
int AnimateEffect::Init(ID3D11Device* d3dDevice,const std::wstring& filename)
{
	if(!BuildFX(d3dDevice,filename))
		return 0;
	mTech= mFX->GetTechniqueByName("Light3TexSkinned");
	mTechAlphaClip = mFX->GetTechniqueByName("Light3_AClipTexSkinned");

	mWorld= mFX->GetVariableByName("gWorld")->AsMatrix();
	mWorldInvTranspose= mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	mWorldViewProj= mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	mTexTransform= mFX->GetVariableByName("gTexTransform")->AsMatrix();
	mBoneTransforms= mFX->GetVariableByName("gBoneTransforms")->AsMatrix();

	mEyePosW          = mFX->GetVariableByName("gEyePosW")->AsVector();

	mDirLight         = mFX->GetVariableByName("gDirLight");
	mMaterial         = mFX->GetVariableByName("gMaterial");

	mDiffuseMap        = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	return 1;
}