#ifndef BASICMODEL_H
#define BASICMODEL_H

#include "MeshGeometry.h"
#include "TextureMgr.h"
#include"..\Common\Geometry.h"

class BasicModel
{
public:
	BasicModel(ID3D11Device* device, TextureMgr& texMgr, const std::string& modelFilename, const std::wstring& texturePath);
	BasicModel(ID3D11Device* device, TextureMgr& texMgr, const std::wstring& texFileName, float width, float depth, float height);
	~BasicModel();

	void DrawIndex(ID3D11DeviceContext* dc, ID3DX11EffectTechnique* tech, ID3DX11EffectTechnique* techAlphaClip, UINT passIndex, ID3DX11EffectVariable* pEffectMat, ID3DX11EffectShaderResourceVariable* pEffectSRV);


	UINT SubsetCount;

	std::vector<Material> Mat;
	std::vector<ID3D11ShaderResourceView*> DiffuseMapSRV;

	// Keep CPU copies of the mesh data to read from.  
	std::vector<Vertex::PosNormTex> Vertices;
	std::vector<USHORT> Indices;
	std::vector<MeshGeometry::Subset> Subsets;
	std::vector<int> IsAlphaClips;

	MeshGeometry ModelMesh;
};

class BasicModelInstance
{
public:
	float GetScale(){ return Scale; }
	void SetPosition(float x, float y, float z);
	XMFLOAT4X4& GetWorld(){ return World; }
	void SetModel(BasicModel* model);
	BasicModel* GetModel(){ return Model; }

	BasicModel* Model;
	XMFLOAT3 Position;
protected:
	XMFLOAT4X4 World;
	float Scale;
};

#endif // SKINNEDMODEL_H