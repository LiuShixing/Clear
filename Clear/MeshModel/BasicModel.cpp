#include "BasicModel.h"
#include "LoadM3d.h"

BasicModel::BasicModel(ID3D11Device* device, TextureMgr& texMgr, const std::string& modelFilename, const std::wstring& texturePath)
{
	std::vector<M3dMaterial> mats;
	M3DLoader m3dLoader;
	m3dLoader.LoadM3d(modelFilename, Vertices, Subsets, mats);

	ModelMesh.SetVertices(device, &Vertices[0], Vertices.size());
	ModelMesh.SetIndices(device, &Indices[0], Indices.size());
	ModelMesh.SetSubsetTable(Subsets);

	SubsetCount = mats.size();

	for(UINT i = 0; i < SubsetCount; ++i)
	{
		Mat.push_back(mats[i].Mat);

		ID3D11ShaderResourceView* diffuseMapSRV = texMgr.CreateTexture(texturePath + mats[i].DiffuseMapName);
		DiffuseMapSRV.push_back(diffuseMapSRV);
		IsAlphaClips.push_back(mats[i].AlphaClip);
	}
}
BasicModel::BasicModel(ID3D11Device* device, TextureMgr& texMgr, const std::wstring& texFileName, float width, float depth, float height)
{
	float hW = width / 2.0f;
	float hD = depth / 2.0f;
	float hH = height / 2.0f;

	Vertices.resize(24);
	//前
	Vertices[0].pos = XMFLOAT3(-hW, -hH, -hD); Vertices[0].tex = XMFLOAT2(0.0f, 1.0f);
	Vertices[1].pos = XMFLOAT3(-hW, hH, -hD); Vertices[1].tex = XMFLOAT2(0.0f, 0.0f);
	Vertices[2].pos = XMFLOAT3(hW, hH, -hD); Vertices[2].tex = XMFLOAT2(1.0f, 0.0f);
	Vertices[3].pos = XMFLOAT3(hW, -hH, -hD); Vertices[3].tex = XMFLOAT2(1.0f, 1.0f);
	//后                                                
	Vertices[4].pos = XMFLOAT3(hW, -hH, hD); Vertices[4].tex = XMFLOAT2(0.0f, 1.0f);
	Vertices[5].pos = XMFLOAT3(hW, hH, hD); Vertices[5].tex = XMFLOAT2(0.0f, 0.0f);
	Vertices[6].pos = XMFLOAT3(-hW, hH, hD); Vertices[6].tex = XMFLOAT2(1.0f, 0.0f);
	Vertices[7].pos = XMFLOAT3(-hW, -hH, hD); Vertices[7].tex = XMFLOAT2(1.0f, 1.0f);
	//左
	Vertices[8].pos = XMFLOAT3(-hW, -hH, hD); Vertices[8].tex = XMFLOAT2(0.0f, 1.0f);
	Vertices[9].pos = XMFLOAT3(-hW, hH, hD); Vertices[9].tex = XMFLOAT2(0.0f, 0.0f);
	Vertices[10].pos = XMFLOAT3(-hW, hH, -hD); Vertices[10].tex = XMFLOAT2(1.0f, 0.0f);
	Vertices[11].pos = XMFLOAT3(-hW, -hH, -hD); Vertices[11].tex = XMFLOAT2(1.0f, 1.0f);
	//右
	Vertices[12].pos = XMFLOAT3(hW, -hH, -hD); Vertices[12].tex = XMFLOAT2(0.0f, 1.0f);
	Vertices[13].pos = XMFLOAT3(hW, hH, -hD); Vertices[13].tex = XMFLOAT2(0.0f, 0.0f);
	Vertices[14].pos = XMFLOAT3(hW, hH, hD); Vertices[14].tex = XMFLOAT2(1.0f, 0.0f);
	Vertices[15].pos = XMFLOAT3(hW, -hH, hD); Vertices[15].tex = XMFLOAT2(1.0f, 1.0f);
	//上
	Vertices[16].pos = XMFLOAT3(-hW, hH, -hD); Vertices[16].tex = XMFLOAT2(0.0f, 1.0f);
	Vertices[17].pos = XMFLOAT3(-hW, hH, hD); Vertices[17].tex = XMFLOAT2(0.0f, 0.0f);
	Vertices[18].pos = XMFLOAT3(hW, hH, hD); Vertices[18].tex = XMFLOAT2(1.0f, 0.0f);
	Vertices[19].pos = XMFLOAT3(hW, hH, -hD); Vertices[19].tex = XMFLOAT2(1.0f, 1.0f);
	//下
	Vertices[20].pos = XMFLOAT3(-hW, -hH, hD); Vertices[20].tex = XMFLOAT2(0.0f, 1.0f);
	Vertices[21].pos = XMFLOAT3(-hW, -hH, -hD); Vertices[21].tex = XMFLOAT2(0.0f, 0.0f);
	Vertices[22].pos = XMFLOAT3(hW, -hH, -hD); Vertices[22].tex = XMFLOAT2(1.0f, 0.0f);
	Vertices[23].pos = XMFLOAT3(hW, -hH, hD); Vertices[23].tex = XMFLOAT2(1.0f, 1.0f);
	for (UINT i = 0; i<24; ++i)
	{
		Vertices[i].normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
	//index
	USHORT indices[36] = {
		//前
		0, 1, 2, 0, 2, 3,
		//后
		4, 5, 6, 4, 6, 7,
		//左
		8, 9, 10, 8, 10, 11,
		//右
		12, 13, 14, 12, 14, 15,
		//上
		16, 17, 18, 16, 18, 19,
		//下
		20, 21, 22, 20, 22, 23 };

	Indices.resize(36);
	for (UINT i = 0; i<36; ++i)
	{
		Indices[i] = indices[i];
	}

	//计算法向量
	g_CaculateNormalUSHORT(&Vertices[0], 24, indices, 36);

	ModelMesh.SetVertices(device, &Vertices[0], Vertices.size());
	ModelMesh.SetIndices(device, &Indices[0], Indices.size());

	Subsets.resize(1);
	Subsets[0].Id = 0;
	Subsets[0].VertexCount = 24;
	Subsets[0].VertexStart = 0;
	Subsets[0].FaceCount = 12;
	Subsets[0].FaceStart = 0;
	ModelMesh.SetSubsetTable(Subsets);

	SubsetCount = 1;

	Mat.resize(1);
	float v = 0.7f;
	Mat[0].Ambient = XMFLOAT4(v, v, v, 1.0f);
	Mat[0].Diffuse = XMFLOAT4(v, v, v, 1.0f);
	Mat[0].Reflect = XMFLOAT4(v, v, v, 1.0f);
	Mat[0].Specular = XMFLOAT4(v, v, v, 1.0f);

	ID3D11ShaderResourceView* diffuseMapSRV = texMgr.CreateTexture(texFileName);
	DiffuseMapSRV.push_back(diffuseMapSRV);
	IsAlphaClips.push_back(0);
}

BasicModel::~BasicModel()
{
}
void BasicModel::DrawIndex(ID3D11DeviceContext* dc, ID3DX11EffectTechnique* tech, ID3DX11EffectTechnique* techAlphaClip, UINT passIndex, ID3DX11EffectVariable* pEffectMat, ID3DX11EffectShaderResourceVariable* pEffectSRV)
{
	UINT offset = 0;
	dc->IASetVertexBuffers(0, 1, &ModelMesh.mVB, &ModelMesh.mVertexStride, &offset);
	dc->IASetIndexBuffer(ModelMesh.mIB, ModelMesh.mIndexBufferFormat, 0);
	for (int subset = 0; subset < Subsets.size(); subset++)
	{
		pEffectMat->SetRawValue(&Mat[subset], 0, sizeof(Material));

		pEffectSRV->SetResource(DiffuseMapSRV[subset]);
		if (IsAlphaClips[subset])
		{
			techAlphaClip->GetPassByIndex(passIndex)->Apply(0, dc);

		}
		else
		{
			tech->GetPassByIndex(passIndex)->Apply(0, dc);
		}
		ModelMesh.DrawIndexed(dc, subset);
	}
}
void BasicModelInstance::SetPosition(float x, float y, float z)
{
	Position.x = x;
	Position.y = y;
	Position.z = z;
	XMMATRIX worldMat = XMMatrixTranslation(x, y, z);

	XMStoreFloat4x4(&World, worldMat);
}
void BasicModelInstance::SetModel(BasicModel* model)
{
	Model = model;
}
