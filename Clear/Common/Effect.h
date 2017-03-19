#pragma once

#include"d3dUtil.h"
#include"Camera.h"

#define WHITE     0
	 
#define BLACK           1
#define RED             2
#define GREEN           3
#define BLUE            4
#define YELLOW           5
#define CYAN            6//蓝绿色
#define MAGENTA         7//品红
#define SILVER          8//银
#define LIGHT_STEEL_BLUE   9//
#define SKYBLUE           10


void g_SetColor(UINT color,float intensity,XMFLOAT4& A,XMFLOAT4& D,XMFLOAT4& S);
int g_KeyDown(int vKey);

struct DirectLight
{
   DirectLight()
   {
	   ZeroMemory(this,sizeof(this));
	   Direction =	XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	   SetColor(0,0.5f);
   }
   void SetColor(UINT color,float intensity);

   XMFLOAT4 Ambient;
   XMFLOAT4 Diffuse;
   XMFLOAT4 Specular;
   XMFLOAT3 Direction;
   float    pad;//用来凑成4维
};
struct PointLight
{
   PointLight()
   {
	   ZeroMemory(this,sizeof(this));
	   Range=100.0f;
	   Att=XMFLOAT3(0.0f,0.1f,0.0f);
	   SetColor(0,0.5f);
   }
   void SetColor(UINT color,float intensity);

   XMFLOAT4 Ambient;
   XMFLOAT4 Diffuse;
   XMFLOAT4 Specular;

   XMFLOAT3 Position;
   float    Range;//范围

   XMFLOAT3 Att;//衰减
   float    pad;
};
struct SpotLight
{
   SpotLight()
   {
	   ZeroMemory(this,sizeof(this));
	   Position=XMFLOAT3(0.0f,0.0f,-50.0f);
	   Direction=XMFLOAT3(0.0f,0.0f,1.0f);
	   Att=XMFLOAT3(1.0f,0.0f,0.0f);
	   Spot=96.0f;
	   Range=10000.0f;
	   SetColor(0,0.5f);
   }
   void SetColor(UINT color,float intensity);

   XMFLOAT4 Ambient;
   XMFLOAT4 Diffuse;
   XMFLOAT4 Specular;

   XMFLOAT3 Position;
   float    Range;

   XMFLOAT3 Direction;
   float    Spot;//控制圆锥

   XMFLOAT3 Att;
   float    pad;
};
struct Material
{
   Material()
   {//默认材质
	   ZeroMemory(this,sizeof(this));
	   float moren=1.0f;
	   Ambient  = XMFLOAT4(moren, moren, moren, 1.0f);
 	   Diffuse  = XMFLOAT4(moren, moren, moren, 1.0f);
	   Specular = XMFLOAT4(moren, moren, moren, 1.0f);
   }
   void SetMat(UINT color,float intensity);
   XMFLOAT4 Ambient;
   XMFLOAT4 Diffuse;
   XMFLOAT4 Specular;//w=SpecPower
   XMFLOAT4 Reflect;
};

class Blend
{
public:
	
	static int InitAll(ID3D11Device*   pd3dDevice);

	static ID3D11BlendState*      mTransparentBS;
	static ID3D11BlendState*      mNoColorWBS;
};
class Stencil
{
public:

	ID3D11Device*              md3dDevice;

	static int InitAll(ID3D11Device*   d3dDevice);
	
	static ID3D11DepthStencilState*     mNoDepthButStencilSS;
	static ID3D11DepthStencilState*     mDrawReflectSS;
	static ID3D11DepthStencilState*     mNoDoubleBlendSS;

	
};


class RasterizerState
{
public:
	static int InitAll(ID3D11Device* device);

public:
	static ID3D11RasterizerState*   mNoBackRS;
	static ID3D11RasterizerState*   mWireframeRS;
	static ID3D11RasterizerState*   mCounterClockFrontRS;
};
struct Fog
{
   int      IsFog;
   int       Num;
   float      FogStart;
   float      FogRange;
   XMFLOAT4   FogColor;
};


class Blur
{
public:
	Blur();
	~Blur();

	int   CreateSomeView(ID3D11Device* d3dDevice,UINT ClientWidth,UINT ClientHeight);
	int   BuildFX(LPCWSTR effectFileName );
	void  DoBlur(ID3D11DeviceContext* dc,UINT blurCount);
public:
	ID3D11Device*                              md3dDevice;

	::ID3D11ShaderResourceView*                 mOffscreenSRV;
	::ID3D11RenderTargetView*                   mOffscreenRTV;
	::ID3D11UnorderedAccessView*                mOffscreenUAV;

	::ID3D11ShaderResourceView*                 mBlurredTexSRV;
	::ID3D11UnorderedAccessView*                mBlurredTexUAV;

	
	ID3DX11Effect*                            mBlurFX;
	::ID3DX11EffectShaderResourceVariable*        mBlurFxInput;
	::ID3DX11EffectUnorderedAccessViewVariable*    mBlurFxOutput;
    ::ID3DX11EffectTechnique*                    mHorzBlurTech;
	::ID3DX11EffectTechnique*                    mVertBlurTech;

private:
	UINT                                       mClientWidth;
	UINT                                       mClientHeight;
};

class Sky
{
public:
	Sky();
	~Sky();
	int Init(ID3D11Device* d3dDevice, const std::wstring& cubemapFilename, const std::wstring& hlslFileName);
	int CreateBuffer(ID3D11Device* d3dDevice);
	int BuildFX(ID3D11Device* d3dDevice, const std::wstring& hlslFileName);
	void Draw(ID3D11DeviceContext* dc,Camera& camera);
public:
	UINT mVertexCount;
	UINT mIndiceCount;

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	::ID3DX11Effect*                      mFX;
	::ID3DX11EffectTechnique*              mTech;
	::ID3DX11EffectMatrixVariable*          mFxWorldViewProj;
	ID3DX11EffectShaderResourceVariable*     mFxCubeMapSRV;

	ID3D11ShaderResourceView*     mCubeMapSRV;
};
class Effect
{
public:
	static int BuildFX(ID3D11Device* d3dDevice,const std::wstring& filename);
public:
	static ID3DX11Effect* mFX;
};