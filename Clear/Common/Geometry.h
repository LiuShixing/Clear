#pragma once

#include"d3dUtil.h"
#include"Effect.h"
#include<iterator>
#include "xnacollision.h"

namespace Vertex
{

    struct PosNormTex
    {
    	XMFLOAT3 pos;
    	XMFLOAT3 normal;
		XMFLOAT2 tex;
    };
	struct PosNorm
	{
		XMFLOAT3 pos;
    	XMFLOAT3 normal;
	};
	struct PosSize
	{
		XMFLOAT3 pos;
		XMFLOAT2 size;
	};
	struct Pos
    {
		XMFLOAT3 pos;
    };
	struct InstanceDate
	{
		XMFLOAT4X4 world;
		XMFLOAT4   color;
	};
	struct PosNormalTexTan
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
		XMFLOAT4 TangentU;
	};
	struct PosNormalTexTanSkinned
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
		XMFLOAT4 TangentU;
		XMFLOAT3 Weights;
		BYTE BoneIndices[4];
	};
	struct PosNormalTexSkinned
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
		XMFLOAT3 Weights;  //第四个值用1.0f-去前三个值得到
		BYTE BoneIndices[4];
	};
	struct Terrain
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
		XMFLOAT2 BoundsY;
	};
}
void g_Vector3Add_Equal(XMFLOAT3& V1,const XMFLOAT3& V2);
void g_CaculateNormal(Vertex::PosNormTex* pVer, UINT numVer, UINT* pIn, UINT numIndex);
void g_CaculateNormalUSHORT(Vertex::PosNormTex* pVer, UINT numVer, USHORT* pIn, UINT numIndex);


//---------------------------------------------------------------------------------------------
//              创建输入布局时 Tech 指向effect文件的VertexIn 必须和所建布局组成一样
//--------------------------------------------------------------------------------------------
class InputLayout
{
public:
	
	static int InitPosNormTex(ID3D11Device* pd3dDevice,ID3DX11EffectTechnique*  Tech);
	static int InitPosSize(ID3D11Device* pd3dDevice,ID3DX11EffectTechnique*  Tech);
	static int InitPos(ID3D11Device* pd3dDevice,ID3DX11EffectTechnique*  Tech);
	static int InitInstance(ID3D11Device* pd3dDevice,ID3DX11EffectTechnique*  Tech);
	static int InitPosNormalTexTan(ID3D11Device* pd3dDevice,ID3DX11EffectTechnique*  Tech);
	static int InitPosNormalTexSkinned(ID3D11Device* pd3dDevice, ID3DX11EffectTechnique*  Tech);
	static int InitPosNormalTexTanSkinned(ID3D11Device* pd3dDevice,ID3DX11EffectTechnique*  Tech);
	static int InitTerrain(ID3D11Device* pd3dDevice,ID3DX11EffectTechnique*  Tech);
public:
	//-----------------------------------------------------------------------
   //             C++静态成员变量必须初始化
   //-----------------------------------------------------------------------
	static ID3D11InputLayout*       mPosNormTex;
	static ID3D11InputLayout*       mPosSize;
	static ID3D11InputLayout*       mPos;
	static ID3D11InputLayout*       mInstance;
	static ID3D11InputLayout*       mPosNormalTexTan;
	static ID3D11InputLayout*       mPosNormalTexSkinned;
	static ID3D11InputLayout*       mPosNormalTexTanSkinned;
	static ID3D11InputLayout*       mTerrain;
};

class Box
{
public:
	Box();
	~Box();
	XMFLOAT4X4& GetWorld()
	{
		return mWorld;
	}
	XMFLOAT4X4& GetTexTransform()
	{
		return mTexTransform;
	}
	float& GetWidth()
	{
		if(mWidth==0)
	    	MessageBox(0, L"failed! box isnot created ", 0, 0);
		return mWidth;
	}
	float& GetDepth()
	{
		if(mWidth==0)
	    	MessageBox(0, L"failed! box isnot created ", 0, 0);
		return mDepth;
	}
	float& GetHeight()
	{
		if(mWidth==0)
	    	MessageBox(0, L"failed! box isnot created ", 0, 0);
		return mHeight;
	}
	UINT& GetNumVer()
	{
		return mNumVer;
	}
	UINT& GetNumFace()
	{
		return mNumFace;
	}
	int CreateBuffer();
	void SetWorldMatrix(float offsetX,float offsetY,float offsetZ,XMFLOAT3 axis=XMFLOAT3(0.0f,0.0f,0.0f),float angleRotA=0);
	int CreateBox(ID3D11Device* pd3dDevice,float width,float height,float depth);
	int CreateShaderRV(LPCWSTR fileName);
private:
	ID3D11Device*           mpd3dDevice;
public:
	ID3D11Buffer*              mpVertexBuffer;
	ID3D11Buffer*              mpIndexBuffer;
	ID3D11ShaderResourceView*   mShaderSView;
private:
	float       mWidth;
	float       mDepth;
	float       mHeight;
	
	UINT       mNumVer;
	UINT       mNumFace;
public:
	XMFLOAT4X4  mWorld;

	XMFLOAT4X4  mTexTransform;
public:
	std::vector<Vertex::PosNormTex> mVertexs;
	std::vector<UINT>            mIndices;
	Material                    mMat;
};
class Skull
{
public:
	Skull();
	~Skull();
	XMFLOAT4X4& GetWorld()
	{
		return mWorld;
	}
	UINT& GetNumVer()
	{
		return mNumVer;
	}
	UINT& GetNumFace()
	{
		return mNumFace;
	}
	int CreateBuffer();
	int CreateInputLayout(ID3DX11EffectTechnique*  Tech);
	void SetWorldMatrix(float offsetX,float offsetY,float offsetZ,XMFLOAT3 axis=XMFLOAT3(0.0f,0.0f,0.0f),float angleRotA=0);
	int CreateSkull(ID3D11Device* pd3dDevice);
private:
	ID3D11Device*           mpd3dDevice;
public:
	ID3D11Buffer*           mpVertexBuffer;
	ID3D11Buffer*           mpIndexBuffer;
	ID3D11InputLayout*       mpInputLayout;
private:
	UINT        mNumVer;
	UINT        mNumFace;
public:
	std::vector<Vertex::PosNorm> mVertexs;
	std::vector<UINT>             mIndices;
	Material                     mMat;
	XMFLOAT4X4                   mWorld;

	XNA::AxisAlignedBox           mSkullBox;
};
class Terrain
{
public:
	Terrain();
	~Terrain();
	XMFLOAT4X4& GetWorld()
	{
		return mWorld;
	}
	XMFLOAT4X4& GetTexTransform()
	{
		return mTexTransform;
	}
	UINT& GetNumVer()
	{
		return mNumVer;
	}
	UINT& GetNumFace()
	{
		return mNumFace;
	}
	float& GetWidth()
	{
		return mWidth;
	}
	float& GetDepth()
	{
		return mDepth;
	}
	void SetWorldMatrix(float offsetX,float offsetY,float offsetZ,XMFLOAT3 axis=XMFLOAT3(0.0f,0.0f,0.0f),float angleRotA=0);
	int Init(ID3D11Device* pd3dDevice,float width,float depth,UINT row,UINT col,LPCWSTR fileName);
	int CreateBuffer();
//	int CreateShaderRV(LPCWSTR fileName);
private:
	ID3D11Device*           mpd3dDevice;
public:
	ID3D11Buffer*             mpVertexBuffer;
	ID3D11Buffer*             mpIndexBuffer;
	ID3D11ShaderResourceView*   mShaderSView;
private:
	float        mWidth;
	float        mDepth;
	UINT        mRow;//行
	UINT        mCol;//列
	UINT        mNumVer;
	UINT        mNumFace;
public:
	XMFLOAT4X4   mWorld;
	XMFLOAT4X4  mTexTransform;
public:
	std::vector<Vertex::PosNormTex> mVertexs;
	std::vector<UINT>            mIndices;
	Material                    mMat;
};

class FullscreenQuad
{
public:
	FullscreenQuad();
	~FullscreenQuad();
	int CreateBuffer(ID3D11Device* pd3dDevice);
private:
	ID3D11Device*           mpd3dDevice;
public:
	std::vector<Vertex::PosNormTex> mVertexs;
	std::vector<UINT>            mIndices;

	ID3D11Buffer*             mpVertexBuffer;
	ID3D11Buffer*             mpIndexBuffer;
};

class Sphere
{
public:
	Sphere();
	~Sphere();
	int CreateBuffer(ID3D11Device* pd3dDevice,float radius);
	void SetWorld(float x,float y,float z);
	float GetRadius()
	{
		return mRadius;
	}
private:
	ID3D11Device*           mpd3dDevice;

	float                  mRadius;
	UINT                   mSliceCount;
	UINT                   mStackCount;
public:
	std::vector<Vertex::PosNormTex> mVertexs;
	std::vector<UINT>            mIndices;

	XMFLOAT4X4                mWorld;

	ID3D11Buffer*             mpVertexBuffer;
	ID3D11Buffer*             mpIndexBuffer;
};