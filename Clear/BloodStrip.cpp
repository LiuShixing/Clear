#include"BloodStrip.h"

int BloodStrip::Init(ID3D11Device* pd3dDevice, const std::wstring& head, const std::wstring& BloodBaground, const std::wstring& Blood, float left, float top, float width, float height)
{
	mVertexs.resize(12);
	mIndices.resize(18);

	//本地坐标系原点在矩形左上角
	//    1    2
	//    0    3
	//血条框
	float bz = 0.1f;
	float x = 0.0f,y=0.0f;
	mVertexs[0].pos = XMFLOAT3(x, y-height, bz);
	mVertexs[1].pos = XMFLOAT3(x, y, bz);
	mVertexs[2].pos = XMFLOAT3(x+width, y, bz);
	mVertexs[3].pos = XMFLOAT3(x+width, y-height, bz);

	mVertexs[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	mVertexs[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	mVertexs[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	mVertexs[3].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	mVertexs[0].tex = XMFLOAT2(0.0f, 1.0f);
	mVertexs[1].tex = XMFLOAT2(0.0f, 0.0f);
	mVertexs[2].tex = XMFLOAT2(1.0f, 0.0f);
	mVertexs[3].tex = XMFLOAT2(1.0f, 1.0f);

	mIndices[0] = 0;
	mIndices[1] = 1;
	mIndices[2] = 2;

	mIndices[3] = 0;
	mIndices[4] = 2;
	mIndices[5] = 3;

	//血条
	float z = 0.0f;
	mVertexs[4].pos = XMFLOAT3(x, y-height, z);
	mVertexs[5].pos = XMFLOAT3(x, y, z);
	mVertexs[6].pos = XMFLOAT3(x+width, y, z);
	mVertexs[7].pos = XMFLOAT3(x+width, y-height, z);

	mVertexs[4].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	mVertexs[5].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	mVertexs[6].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	mVertexs[7].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	mVertexs[4].tex = XMFLOAT2(0.0f, 1.0f);
	mVertexs[5].tex = XMFLOAT2(0.0f, 0.0f);
	mVertexs[6].tex = XMFLOAT2(1.0f, 0.0f);
	mVertexs[7].tex = XMFLOAT2(1.0f, 1.0f);

	mIndices[6] = 4;
	mIndices[7] = 5;
	mIndices[8] = 6;

	mIndices[9] = 4;
	mIndices[10] = 6;
	mIndices[11] = 7;

	XMStoreFloat4x4(&mBagroundWorld, XMMatrixTranslation(left, top, bz));
	XMStoreFloat4x4(&mBloodWorld, XMMatrixTranslation(left, top, z));

	//头像
	float headW = height*1.6f;
	mVertexs[8].pos = XMFLOAT3(x - headW, y - height, bz);
	mVertexs[9].pos = XMFLOAT3(x - headW, y+height, bz);
	mVertexs[10].pos = XMFLOAT3(x , y+height, bz);
	mVertexs[11].pos = XMFLOAT3(x, y - height, bz);

	mVertexs[8].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	mVertexs[9].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	mVertexs[10].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	mVertexs[11].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	mVertexs[8].tex = XMFLOAT2(0.0f, 1.0f);
	mVertexs[9].tex = XMFLOAT2(0.0f, 0.0f);
	mVertexs[10].tex = XMFLOAT2(1.0f, 0.0f);
	mVertexs[11].tex = XMFLOAT2(1.0f, 1.0f);

	mIndices[12] = 8;
	mIndices[13] = 9;
	mIndices[14] = 10;

	mIndices[15] = 8;
	mIndices[16] = 10;
	mIndices[17] = 11;
	


	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::PosNormTex) * mVertexs.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &mVertexs[0];

	if (FAILED(pd3dDevice->CreateBuffer(&vbd, &vinitData, &mpVertexBuffer)))
		return 0;

	D3D11_BUFFER_DESC iDesc = { 0 };
	iDesc.ByteWidth = mIndices.size()* sizeof(UINT);
	iDesc.Usage = D3D11_USAGE_IMMUTABLE;
	iDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA ibdata = { 0 };
	ibdata.pSysMem = &mIndices[0];
	if (FAILED(pd3dDevice->CreateBuffer(&iDesc, &ibdata, &mpIndexBuffer)))
		return 0;

	if (FAILED(D3DX11CreateShaderResourceViewFromFile(pd3dDevice, BloodBaground.c_str(), 0, 0, &mBagroundSRV, 0)))
		return 0;
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(pd3dDevice, Blood.c_str(), 0, 0, &mBloodSRV, 0)))
		return 0; 
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(pd3dDevice, head.c_str(), 0, 0, &mLeonaHeadSRV, 0)))
		return 0;
	return 1;
}
void BloodStrip::Draw(ID3D11DeviceContext* dc, float bloodScaling)
{
	if (BasicEffect::mTechAlphaClip == NULL || InputLayout::mPosNormTex==NULL)
		return;
	UINT stride = sizeof(Vertex::PosNormTex);
	UINT offset = 0;

	XMMATRIX identity = XMMatrixIdentity();


	dc->IASetInputLayout(InputLayout::mPosNormTex);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DX11_TECHNIQUE_DESC techDesc;
	BasicEffect::mTechAlphaClip->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		dc->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
		dc->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//draw blood framework
		XMMATRIX WORLD = XMLoadFloat4x4(&mBagroundWorld);

		XMMATRIX TW = WORLD;
		TW.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMVECTOR det = XMMatrixDeterminant(TW);
		XMMATRIX WorldInv = XMMatrixTranspose(XMMatrixInverse(&det, TW));

		BasicEffect::mWorld->SetMatrix(reinterpret_cast<float*>(&WORLD));
		BasicEffect::mWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&WorldInv));
		BasicEffect::mWorldViewProj->SetMatrix(reinterpret_cast<float*>(&WORLD));//!!!!!!!!!!!!!!!!!!!!!
		BasicEffect::mTexTransform->SetMatrix(reinterpret_cast<float*>(&identity));

		BasicEffect::mMaterial->SetRawValue(&mMaterial, 0, sizeof(mMaterial)); //设置材质 免得受阴影材质影响

		BasicEffect::mDiffuseMap->SetResource(mBagroundSRV);
		BasicEffect::mTechAlphaClip->GetPassByIndex(p)->Apply(0, dc);
		dc->DrawIndexed(6, 0, 0);

		//draw head
		BasicEffect::mDiffuseMap->SetResource(mLeonaHeadSRV);
		BasicEffect::mTechAlphaClip->GetPassByIndex(p)->Apply(0, dc);
		dc->DrawIndexed(6, 12, 0);

		//draw blood
		WORLD = XMLoadFloat4x4(&mBloodWorld);
		WORLD = XMMatrixScaling(bloodScaling, 1.0f, 1.0f)*WORLD;   //根据血量缩放长度

		TW = WORLD;
		TW.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		det = XMMatrixDeterminant(TW);
		WorldInv = XMMatrixTranspose(XMMatrixInverse(&det, TW));

		BasicEffect::mWorld->SetMatrix(reinterpret_cast<float*>(&WORLD));
		BasicEffect::mWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&WorldInv));
		BasicEffect::mWorldViewProj->SetMatrix(reinterpret_cast<float*>(&WORLD));//!!!!!!!!!!!!!!!!!!!!!

		BasicEffect::mDiffuseMap->SetResource(mBloodSRV);
		BasicEffect::mTechAlphaClip->GetPassByIndex(p)->Apply(0, dc);
		dc->DrawIndexed(6, 6, 0);

	}
}