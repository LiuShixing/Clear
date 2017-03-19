#include"BasicEffect.h"


ID3DX11EffectTechnique*                  BasicEffect::mTech=NULL;
ID3DX11EffectTechnique*                  BasicEffect::mTechAlphaClip = NULL;
ID3DX11EffectMatrixVariable*             BasicEffect::mWorld=NULL;
ID3DX11EffectMatrixVariable*             BasicEffect::mWorldInvTranspose=NULL;
ID3DX11EffectMatrixVariable*             BasicEffect::mWorldViewProj=NULL;
ID3DX11EffectMatrixVariable*             BasicEffect::mTexTransform=NULL;
ID3DX11EffectVectorVariable*             BasicEffect::mEyePosW=NULL;
ID3DX11EffectVariable*                  BasicEffect::mDirLight=NULL;
ID3DX11EffectVariable*                  BasicEffect::mMaterial=NULL;
ID3DX11EffectShaderResourceVariable*      BasicEffect::mDiffuseMap=NULL;
int BasicEffect::Init(ID3D11Device* d3dDevice,const std::wstring& filename)
{
	if(!BuildFX(d3dDevice,filename))
		return 0;
	mTech= mFX->GetTechniqueByName("Tech");
	mTechAlphaClip = mFX->GetTechniqueByName("TechAlphaClip");

	mWorld= mFX->GetVariableByName("gWorld")->AsMatrix();
	mWorldInvTranspose= mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	mWorldViewProj= mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	mTexTransform= mFX->GetVariableByName("gTexTransform")->AsMatrix();

	mEyePosW          = mFX->GetVariableByName("gEyePosW")->AsVector();

	mDirLight         = mFX->GetVariableByName("gDirLight");
	mMaterial         = mFX->GetVariableByName("gMaterial");

	mDiffuseMap        = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();

	return 1;
}

ID3DX11EffectTechnique*                  NoColorEffect::mTech = NULL;
ID3DX11EffectMatrixVariable*             NoColorEffect::mWorld = NULL;
ID3DX11EffectMatrixVariable*             NoColorEffect::mWorldInvTranspose = NULL;
ID3DX11EffectMatrixVariable*             NoColorEffect::mWorldViewProj = NULL;
int NoColorEffect::Init(ID3D11Device* d3dDevice, const std::wstring& filename)
{
	if (!BuildFX(d3dDevice, filename))
		return 0;
	mTech = mFX->GetTechniqueByName("BasicDraw");

	mWorld = mFX->GetVariableByName("gWorld")->AsMatrix();
	mWorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	mWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	return 1;
}