#ifndef BLOODSTRIP_H
#define BLOODSTRIP_H

#include"Common\d3dUtil.h"
#include"Common\Effect.h"
#include"Common\Geometry.h"
#include"Effect\BasicEffect.h"

class BloodStrip
{
public:
	//left top width height =[-1,1] , left top血条的左上角坐标
	int Init(ID3D11Device* pd3dDevice, const std::wstring& head, const std::wstring& BloodBaground, const std::wstring& Blood, float left, float top, float width, float height);
	
	//bloodScaling 剩余血的比例
	void Draw(ID3D11DeviceContext* dc,float bloodScaling);
private:
	std::vector<Vertex::PosNormTex> mVertexs;
	std::vector<UINT>            mIndices;
	Material                    mMaterial;
	XMFLOAT4X4                  mBagroundWorld;
	XMFLOAT4X4                  mBloodWorld;

	ID3D11Buffer*             mpVertexBuffer;
	ID3D11Buffer*             mpIndexBuffer;
	ID3D11ShaderResourceView*  mBagroundSRV;
	ID3D11ShaderResourceView*  mBloodSRV;
	ID3D11ShaderResourceView*  mLeonaHeadSRV;
};

#endif