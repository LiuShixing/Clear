#include"Geometry.h"

void g_Vector3Add_Equal(XMFLOAT3& V1,const XMFLOAT3& V2)
{
	V1.x+=V2.x;
	V1.y+=V2.y;
	V1.z+=V2.z;
}
void g_CaculateNormal(Vertex::PosNormTex* pVer, UINT numVer, UINT* pIn, UINT numIndex)
{
	UINT numFace=numIndex/3;
	for(UINT i=0;i<numFace;++i)
	{
		UINT i0=pIn[i*3];
		UINT i1=pIn[i*3+1];
		UINT i2=pIn[i*3+2];

		float x=pVer[i1].pos.x-pVer[i0].pos.x;
		float y=pVer[i1].pos.y-pVer[i0].pos.y;
		float z=pVer[i1].pos.z-pVer[i0].pos.z;

		float x2=pVer[i2].pos.x-pVer[i0].pos.x;
		float y2=pVer[i2].pos.y-pVer[i0].pos.y;
		float z2=pVer[i2].pos.z-pVer[i0].pos.z;
		XMFLOAT3 v0(x,y,z);
		XMFLOAT3 v1(x2,y2,z2);
		XMVECTOR V0=XMLoadFloat3(&v0);
		XMVECTOR V1=XMLoadFloat3(&v1);
		XMVECTOR N=XMVector3Normalize(::XMVector3Cross(V0,V1));
		XMFLOAT3 n;
		XMStoreFloat3(&n,N);
		g_Vector3Add_Equal(pVer[i0].normal,n);
		g_Vector3Add_Equal(pVer[i1].normal,n);
		g_Vector3Add_Equal(pVer[i2].normal,n);
	}
	for(UINT i=0;i<numVer;++i)//Normalize
	{
		XMVECTOR n=XMLoadFloat3(&pVer[i].normal);
		n=::XMVector3Normalize(n);
		::XMStoreFloat3(&pVer[i].normal,n);
	}
}
void g_CaculateNormalUSHORT(Vertex::PosNormTex* pVer, UINT numVer, USHORT* pIn, UINT numIndex)
{
	USHORT numFace = (USHORT) numIndex / 3;
	for (USHORT i = 0; i<numFace; ++i)
	{
		USHORT i0 = pIn[i * 3];
		USHORT i1 = pIn[i * 3 + 1];
		USHORT i2 = pIn[i * 3 + 2];

		float x = pVer[i1].pos.x - pVer[i0].pos.x;
		float y = pVer[i1].pos.y - pVer[i0].pos.y;
		float z = pVer[i1].pos.z - pVer[i0].pos.z;

		float x2 = pVer[i2].pos.x - pVer[i0].pos.x;
		float y2 = pVer[i2].pos.y - pVer[i0].pos.y;
		float z2 = pVer[i2].pos.z - pVer[i0].pos.z;
		XMFLOAT3 v0(x, y, z);
		XMFLOAT3 v1(x2, y2, z2);
		XMVECTOR V0 = XMLoadFloat3(&v0);
		XMVECTOR V1 = XMLoadFloat3(&v1);
		XMVECTOR N = XMVector3Normalize(::XMVector3Cross(V0, V1));
		XMFLOAT3 n;
		XMStoreFloat3(&n, N);
		g_Vector3Add_Equal(pVer[i0].normal, n);
		g_Vector3Add_Equal(pVer[i1].normal, n);
		g_Vector3Add_Equal(pVer[i2].normal, n);
	}
	for (UINT i = 0; i<numVer; ++i)//Normalize
	{
		XMVECTOR n = XMLoadFloat3(&pVer[i].normal);
		n = ::XMVector3Normalize(n);
		::XMStoreFloat3(&pVer[i].normal, n);
	}
}
//-----------------------------------------------------------------------
//             C++静态成员变量必须初始化
//-----------------------------------------------------------------------
ID3D11InputLayout* InputLayout::mPos = NULL;
ID3D11InputLayout* InputLayout::mPosNormTex = NULL;
ID3D11InputLayout* InputLayout::mInstance = NULL;
ID3D11InputLayout* InputLayout::mPosSize= NULL;
ID3D11InputLayout* InputLayout::mPosNormalTexTan= NULL;
ID3D11InputLayout* InputLayout::mPosNormalTexSkinned = NULL;
ID3D11InputLayout* InputLayout::mPosNormalTexTanSkinned= NULL;
ID3D11InputLayout* InputLayout::mTerrain = NULL;

int InputLayout::InitPosNormTex(ID3D11Device* pd3dDevice,ID3DX11EffectTechnique*  Tech)
{
	if(!Tech)
	{
		MessageBox(0, L"Tech=NULL!! failed", 0, 0);
	}
	D3D11_INPUT_ELEMENT_DESC PosNorTexInputDesc[3]={
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	D3DX11_PASS_DESC PassDesc;
	Tech->GetPassByIndex(0)->GetDesc(&PassDesc);
	if(FAILED(pd3dDevice->CreateInputLayout (PosNorTexInputDesc,3,PassDesc.pIAInputSignature,
		     PassDesc.IAInputSignatureSize,&mPosNormTex)))
		return 0;
	return 1;
}
int InputLayout::InitPosSize(ID3D11Device* pd3dDevice,ID3DX11EffectTechnique*  Tech)
{
	if(!Tech)
	{
		MessageBox(0, L"Tech=NULL!! failed", 0, 0);
	}
	D3DX11_PASS_DESC PassDesc;
	D3D11_INPUT_ELEMENT_DESC PosSizeInputDesc[2]={
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"SIZE",0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	Tech->GetPassByIndex(0)->GetDesc(&PassDesc);
	if(FAILED(pd3dDevice->CreateInputLayout (PosSizeInputDesc,2,PassDesc.pIAInputSignature,
		     PassDesc.IAInputSignatureSize,&mPosSize)))
		return 0;
	return 1;
}
int InputLayout::InitPos(ID3D11Device* pd3dDevice,ID3DX11EffectTechnique*  Tech)
{
	if(!Tech)
	{
		MessageBox(0, L"Tech=NULL!! failed", 0, 0);
	}
	D3DX11_PASS_DESC PassDesc;
	D3D11_INPUT_ELEMENT_DESC PosDesc[1] =       
    {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
	Tech->GetPassByIndex(0)->GetDesc(&PassDesc);
	if(FAILED(pd3dDevice->CreateInputLayout (PosDesc,1,PassDesc.pIAInputSignature,
		     PassDesc.IAInputSignatureSize,&mPos)))
		return 0;
	return 1;
}
int InputLayout::InitInstance(ID3D11Device* pd3dDevice,ID3DX11EffectTechnique*  Tech)
{
	if(!Tech)
	{
		MessageBox(0, L"Tech=NULL!! failed", 0, 0);
	}
	D3DX11_PASS_DESC PassDesc;
	D3D11_INPUT_ELEMENT_DESC InstanceDesc[8] = 
    {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    { "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    { "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    { "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64,  D3D11_INPUT_PER_INSTANCE_DATA, 1 }
    };
	Tech->GetPassByIndex(0)->GetDesc(&PassDesc);
	if(FAILED(pd3dDevice->CreateInputLayout (InstanceDesc,8,PassDesc.pIAInputSignature,
		     PassDesc.IAInputSignatureSize,&mInstance)))
		return 0;
	return 1;
}
int InputLayout::InitPosNormalTexTan(ID3D11Device* pd3dDevice,ID3DX11EffectTechnique*  Tech)
{
	if(!Tech)
	{
		MessageBox(0, L"Tech=NULL!! failed", 0, 0);
	}
	D3DX11_PASS_DESC PassDesc;
	D3D11_INPUT_ELEMENT_DESC PosNormalTexTanDesc[4] = 
   {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TANGENT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
	Tech->GetPassByIndex(0)->GetDesc(&PassDesc);
	if(FAILED(pd3dDevice->CreateInputLayout (PosNormalTexTanDesc,4,PassDesc.pIAInputSignature,
		     PassDesc.IAInputSignatureSize,&mPosNormalTexTan)))
		return 0;
	return 1;
}
int InputLayout::InitPosNormalTexSkinned(ID3D11Device* pd3dDevice, ID3DX11EffectTechnique*  Tech)
{
	if (!Tech)
	{
		MessageBox(0, L"Tech=NULL!! failed", 0, 0);
	}
	D3DX11_PASS_DESC PassDesc;
	D3D11_INPUT_ELEMENT_DESC PosNormalTexSkinnedDesc[5] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	Tech->GetPassByIndex(0)->GetDesc(&PassDesc);
	if (FAILED(pd3dDevice->CreateInputLayout(PosNormalTexSkinnedDesc, 5, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &mPosNormalTexSkinned)))
		return 0;
	return 1;

}
int InputLayout::InitPosNormalTexTanSkinned(ID3D11Device* pd3dDevice,ID3DX11EffectTechnique*  Tech)
{
	if(!Tech)
	{
		MessageBox(0, L"Tech=NULL!! failed", 0, 0);
	}
	D3DX11_PASS_DESC PassDesc;
	D3D11_INPUT_ELEMENT_DESC PosNormalTexTanSkinnedDesc[6] = 
   {
	{"POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL",       0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TANGENT",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"WEIGHTS",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"BONEINDICES",  0, DXGI_FORMAT_R8G8B8A8_UINT,   0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0}
   };
	Tech->GetPassByIndex(0)->GetDesc(&PassDesc);
	if(FAILED(pd3dDevice->CreateInputLayout (PosNormalTexTanSkinnedDesc,6,PassDesc.pIAInputSignature,
		     PassDesc.IAInputSignatureSize,&mPosNormalTexTanSkinned)))
		return 0;
	return 1;

}
int InputLayout::InitTerrain(ID3D11Device* pd3dDevice,ID3DX11EffectTechnique*  Tech)
{
	if(!Tech)
	{
		MessageBox(0, L"Tech=NULL!! failed", 0, 0);
	}
	D3DX11_PASS_DESC PassDesc;
	D3D11_INPUT_ELEMENT_DESC TerrainDesc[3] = 
   {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
   };
	Tech->GetPassByIndex(0)->GetDesc(&PassDesc);
	if(FAILED(pd3dDevice->CreateInputLayout (TerrainDesc,3,PassDesc.pIAInputSignature,
		     PassDesc.IAInputSignatureSize,&mTerrain)))
		return 0;
	return 1;
}
Box::Box():mpd3dDevice(NULL), mpVertexBuffer(NULL), mpIndexBuffer(NULL),mWidth(0),mDepth(0),mHeight(0),
	      mNumFace(12),mNumVer(24)
{
	::XMMATRIX O(::XMMatrixIdentity());															
	::XMStoreFloat4x4(&mWorld,O);
	::XMStoreFloat4x4(&mTexTransform,O);
}
                                                                                                                             
Box::~Box()
{
	ReleaseCOM(mpd3dDevice);
	ReleaseCOM(mpVertexBuffer);
	ReleaseCOM(mpIndexBuffer);
}
int Box::CreateBox(ID3D11Device* pd3dDevice,float width,float height,float depth)                                                               	                                                       
{
	mpd3dDevice=pd3dDevice;
	mWidth=width;
	mDepth=depth;
    mHeight=height;

	if(!CreateBuffer())
		return 0;
	return 1;

}

int Box::CreateBuffer()
{
	float hW=mWidth/2.0f;
	float hD=mDepth/2.0f;
	float hH=mHeight/2.0f;
/*	Geometry::Vertex vertexs[8]={
		//前面4个
	{XMFLOAT3(-hW,-hH,-hD),XMFLOAT3(0.0f,0.0f,0.0f)},  //0-----1------2
    {XMFLOAT3(-hW, hH,-hD),XMFLOAT3(0.0f,0.0f,0.0f)},  //1
    {XMFLOAT3( hW, hH,-hD),XMFLOAT3(0.0f,0.0f,0.0f)},   //2
    {XMFLOAT3( hW,-hH,-hD),XMFLOAT3(0.0f,0.0f,0.0f)}, //3----0------3
       //后面4个
    {XMFLOAT3(-hW,-hH, hD),XMFLOAT3(0.0f,0.0f,0.0f)}, //4    5        6
    {XMFLOAT3(-hW, hH, hD),XMFLOAT3(0.0f,0.0f,0.0f)},//5  
    {XMFLOAT3( hW, hH, hD),XMFLOAT3(0.0f,0.0f,0.0f)}, //6 
    {XMFLOAT3( hW,-hH, hD),XMFLOAT3(0.0f,0.0f,0.0f)} //7   4         7
	};*/
	mVertexs.resize(24);
	//前
	mVertexs[0].pos=XMFLOAT3(-hW,-hH,-hD);mVertexs[0].tex=XMFLOAT2(0.0f,1.0f);              
	mVertexs[1].pos=XMFLOAT3(-hW, hH,-hD);mVertexs[1].tex=XMFLOAT2(0.0f,0.0f);               
	mVertexs[2].pos=XMFLOAT3( hW, hH,-hD);mVertexs[2].tex=XMFLOAT2(1.0f,0.0f);              
	mVertexs[3].pos=XMFLOAT3( hW,-hH,-hD);mVertexs[3].tex=XMFLOAT2(1.0f,1.0f);              
	//后                                                
	mVertexs[4].pos=XMFLOAT3( hW,-hH, hD);mVertexs[4].tex=XMFLOAT2(0.0f,1.0f);               
	mVertexs[5].pos=XMFLOAT3( hW, hH, hD);mVertexs[5].tex=XMFLOAT2(0.0f,0.0f);               
	mVertexs[6].pos=XMFLOAT3(-hW, hH, hD);mVertexs[6].tex=XMFLOAT2(1.0f,0.0f);
	mVertexs[7].pos=XMFLOAT3(-hW,-hH, hD);mVertexs[7].tex=XMFLOAT2(1.0f,1.0f);
	//左
	mVertexs[8].pos=XMFLOAT3(-hW,-hH, hD);mVertexs[8].tex=XMFLOAT2(0.0f,1.0f);               
	mVertexs[9].pos=XMFLOAT3(-hW, hH, hD);mVertexs[9].tex=XMFLOAT2(0.0f,0.0f);               
	mVertexs[10].pos=XMFLOAT3(-hW, hH,-hD);mVertexs[10].tex=XMFLOAT2(1.0f,0.0f);
	mVertexs[11].pos=XMFLOAT3(-hW,-hH,-hD);mVertexs[11].tex=XMFLOAT2(1.0f,1.0f);
	//右
	mVertexs[12].pos=XMFLOAT3(hW,-hH,-hD);mVertexs[12].tex=XMFLOAT2(0.0f,1.0f);               
	mVertexs[13].pos=XMFLOAT3(hW, hH,-hD);mVertexs[13].tex=XMFLOAT2(0.0f,0.0f);               
	mVertexs[14].pos=XMFLOAT3( hW, hH, hD);mVertexs[14].tex=XMFLOAT2(1.0f,0.0f);
	mVertexs[15].pos=XMFLOAT3( hW,-hH, hD);mVertexs[15].tex=XMFLOAT2(1.0f,1.0f);
	//上
	mVertexs[16].pos=XMFLOAT3(-hW,hH,-hD);mVertexs[16].tex=XMFLOAT2(0.0f,1.0f);               
	mVertexs[17].pos=XMFLOAT3(-hW, hH, hD);mVertexs[17].tex=XMFLOAT2(0.0f,0.0f);               
	mVertexs[18].pos=XMFLOAT3( hW, hH, hD);mVertexs[18].tex=XMFLOAT2(1.0f,0.0f);
	mVertexs[19].pos=XMFLOAT3( hW, hH,-hD);mVertexs[19].tex=XMFLOAT2(1.0f,1.0f);
	//下
	mVertexs[20].pos=XMFLOAT3(-hW,-hH, hD);mVertexs[20].tex=XMFLOAT2(0.0f,1.0f);               
	mVertexs[21].pos=XMFLOAT3(-hW,-hH,-hD);mVertexs[21].tex=XMFLOAT2(0.0f,0.0f);               
	mVertexs[22].pos=XMFLOAT3( hW,-hH,-hD);mVertexs[22].tex=XMFLOAT2(1.0f,0.0f);
	mVertexs[23].pos=XMFLOAT3( hW,-hH, hD);mVertexs[23].tex=XMFLOAT2(1.0f,1.0f);
	for(UINT i=0;i<24;++i)
	{
		mVertexs[i].normal=XMFLOAT3(0.0f,0.0f,0.0f);
	}
//index
	UINT indices[36]={
		//前
		0,1,2,  0,2,3,
		//后
		4,5,6,  4,6,7,
		//左
		8,9,10,  8,10,11,
		//右
		12,13,14,  12,14,15,
		//上
		16,17,18,  16,18,19,
		//下
		20,21,22,  20,22,23};

/*	UINT indices[36];
	//前
	indices[0]=0;indices[1]=1;indices[2]=2;
	indices[3]=0;indices[4]=2;indices[5]=3;
	//后
	indices[6]=4;indices[7]=5;indices[8]=6;
	indices[9]=4;indices[10]=6;indices[11]=7;
	//左
	indices[12]=8;indices[13]=9;indices[14]=10;
	indices[15]=8;indices[16]=10;indices[17]=11;
	//右
	indices[18]=12;indices[19]=13;indices[20]=14;
	indices[21]=12;indices[22]=14;indices[23]=15;
	//上
	indices[24]=16;indices[25]=17;indices[26]=18;
	indices[27]=16;indices[28]=18;indices[29]=19;
	//下
	indices[30]=20;indices[31]=21;indices[32]=22;
	indices[33]=20;indices[34]=22;indices[35]=23;
*/
	mIndices.resize(36);
	for(UINT i=0;i<36;++i)
	{
		mIndices[i]=indices[i];
	}

//计算法向量
	g_CaculateNormal(&mVertexs[0],24,indices,36);

	D3D11_BUFFER_DESC vDesc={0};
	vDesc.ByteWidth=24*sizeof(Vertex::PosNormTex);
	vDesc.Usage=D3D11_USAGE_IMMUTABLE;
	vDesc.BindFlags=D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vbdata={0};
	vbdata.pSysMem=&mVertexs[0];

	if(FAILED(mpd3dDevice->CreateBuffer(&vDesc,&vbdata,&mpVertexBuffer)))
		return 0;

	D3D11_BUFFER_DESC iDesc={0};
	iDesc.ByteWidth=36*sizeof(UINT);
	iDesc.Usage=D3D11_USAGE_IMMUTABLE;
	iDesc.BindFlags=D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA ibdata={0};
	ibdata.pSysMem=&mIndices[0];
	if(FAILED(mpd3dDevice->CreateBuffer(&iDesc,&ibdata,&mpIndexBuffer)))
		return 0;
	return 1;
}

void Box::SetWorldMatrix(float offsetX,float offsetY,float offsetZ,XMFLOAT3 axis,float angleRotA)
{
	XMMATRIX WORLD=XMMatrixTranslation(offsetX, offsetY,offsetZ);
	if(angleRotA!=0)
	{
		XMVECTOR A=::XMLoadFloat3(&axis);
		XMMATRIX R=::XMMatrixRotationAxis(A,angleRotA);
		WORLD=R*WORLD;
	}
	XMStoreFloat4x4(&mWorld, WORLD);
}
int Box::CreateShaderRV(LPCWSTR fileName)
{
     if(FAILED(D3DX11CreateShaderResourceViewFromFile(mpd3dDevice, fileName, 0, 0, &mShaderSView, 0 )))
	 {
		 return 0;
	 }
	 return 1;
}
Skull::Skull():mpd3dDevice(NULL), mpVertexBuffer(NULL), mpIndexBuffer(NULL),mpInputLayout(NULL)
{						
	::XMMATRIX O(::XMMatrixIdentity());															
	::XMStoreFloat4x4(&mWorld,O);
}
                                                                                                                             
Skull::~Skull()
{
	ReleaseCOM(mpd3dDevice);
	ReleaseCOM(mpVertexBuffer);
	ReleaseCOM(mpIndexBuffer);
	ReleaseCOM(mpInputLayout);
}
int Skull::CreateBuffer()
{
	std::ifstream fin("Common/skull.txt");
	if(!fin)
	{
		MessageBox(0, L"Common/skull.txt not found.", 0, 0);
		return 0;
	}
	std::string ignore;
	fin>>ignore>>mNumVer>>ignore>>mNumFace;
	if(mNumVer!=31076 || mNumFace!=60339)
		return 0;
	mVertexs.resize(mNumVer);
	fin >> ignore >> ignore >> ignore >> ignore;

	float Infinity=100000.0f;
	XMFLOAT3 vMinf3(+Infinity, +Infinity, +Infinity);
	XMFLOAT3 vMaxf3(-Infinity, -Infinity, -Infinity);
	
	XMVECTOR vMin = XMLoadFloat3(&vMinf3);
	XMVECTOR vMax = XMLoadFloat3(&vMaxf3);

    for(UINT i=0;i<mNumVer;++i)
	{
		fin>>mVertexs[i].pos.x>>mVertexs[i].pos.y>>mVertexs[i].pos.z;
		fin>>mVertexs[i].normal.x>>mVertexs[i].normal.y>>mVertexs[i].normal.z;

		XMVECTOR P = XMLoadFloat3(&mVertexs[i].pos);
		
		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}

	XMStoreFloat3(&mSkullBox.Center, 0.5f*(vMin+vMax));
	XMStoreFloat3(&mSkullBox.Extents, 0.5f*(vMax-vMin));

//mIndices
	UINT numIndex=mNumFace*3;
	mIndices.resize(numIndex);
	fin>>ignore>>ignore>>ignore;
	for(UINT i=0;i<mNumFace;++i)
	{
		fin>>mIndices[i*3+0]>>mIndices[i*3+1]>>mIndices[i*3+2];
	}
	fin.close();

	D3D11_BUFFER_DESC vDesc={0};
	vDesc.ByteWidth=mNumVer*sizeof(Vertex::PosNorm);
	vDesc.Usage=D3D11_USAGE_IMMUTABLE;
	vDesc.BindFlags=D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vbdata={0};
	vbdata.pSysMem=&mVertexs[0];
	
	if(FAILED(mpd3dDevice->CreateBuffer(&vDesc,&vbdata,&mpVertexBuffer)))
		return 0;
	
	D3D11_BUFFER_DESC iDesc={0};
	iDesc.ByteWidth=numIndex*sizeof(UINT);
	iDesc.Usage=D3D11_USAGE_IMMUTABLE;
	iDesc.BindFlags=D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA ibdata={0};
	ibdata.pSysMem=&mIndices[0];
	if(FAILED(mpd3dDevice->CreateBuffer(&iDesc,&ibdata,&mpIndexBuffer)))
		return 0;

	return 1;
}
int Skull::CreateInputLayout(ID3DX11EffectTechnique*  Tech)
{
//	if(!g_CreateInputLayout(mpd3dDevice,Tech,mpInputLayout))
//		return 0;
	D3D11_INPUT_ELEMENT_DESC inputDesc[2]={
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	D3DX11_PASS_DESC passDesc;
	Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	if(FAILED(mpd3dDevice->CreateInputLayout (inputDesc,2,passDesc.pIAInputSignature,
		     passDesc.IAInputSignatureSize,&mpInputLayout)))
		return 0;
   	return 1;
}
void Skull::SetWorldMatrix(float offsetX,float offsetY,float offsetZ,XMFLOAT3 axis,float angleRotA)
{
	XMMATRIX WORLD=XMMatrixTranslation(offsetX, offsetY,offsetZ);
	if(angleRotA!=0)
	{
		XMVECTOR A=::XMLoadFloat3(&axis);
		XMMATRIX R=::XMMatrixRotationAxis(A,angleRotA);
		WORLD=R*WORLD;
	}
	XMStoreFloat4x4(&mWorld, WORLD);
}
int Skull::CreateSkull(ID3D11Device* pd3dDevice)
{
	mpd3dDevice=pd3dDevice;
	if(!CreateBuffer())
		return 0;
	return 1;
}
Terrain::Terrain(): mpd3dDevice(NULL),mpVertexBuffer(NULL),mpIndexBuffer(NULL),
	    mWidth(0),mDepth(0),mRow(0),mNumVer(0),mNumFace(0)
{
	::XMMATRIX O(::XMMatrixIdentity());															
	::XMStoreFloat4x4(&mWorld,O);
	::XMStoreFloat4x4(&mTexTransform,O);
}
Terrain::~Terrain()
{
	ReleaseCOM(mpd3dDevice);
	ReleaseCOM(mpVertexBuffer);
	ReleaseCOM(mpIndexBuffer);
	
}

void Terrain::SetWorldMatrix(float offsetX,float offsetY,float offsetZ,XMFLOAT3 axis,float angleRotA)
{
	XMMATRIX WORLD=XMMatrixTranslation(offsetX, offsetY,offsetZ);
	if(angleRotA!=0)
	{
		XMVECTOR A=::XMLoadFloat3(&axis);
		XMMATRIX R=::XMMatrixRotationAxis(A,angleRotA);
		WORLD=R*WORLD;
	}
	XMStoreFloat4x4(&mWorld, WORLD);
}
int Terrain::Init(ID3D11Device* pd3dDevice,float width,float depth,UINT row,UINT col,LPCWSTR fileName)
{
	mpd3dDevice=pd3dDevice;

	mWidth=width;
	mDepth=depth;
	mRow=row;
	mCol=col;


	if(!CreateBuffer())
		return 0;

    if(FAILED(D3DX11CreateShaderResourceViewFromFile(mpd3dDevice, fileName, 0, 0, &mShaderSView, 0 )))
    {
		 return 0;
    }
	
	return 1;
}
int Terrain::CreateBuffer()
{
	mNumVer=mCol*mRow;
	mNumFace=(mRow-1)*(mCol-1)*2;

	float halfW=mWidth/2.0f;
	float halfD=mDepth/2.0f;

	float dx=mWidth/(mCol-1);
	float dz=mDepth/(mRow-1);

	mVertexs.resize(mNumVer);

	float du=1.0f/(mCol-1);
	float dv=1.0f/(mRow-1);

	for(UINT i=0;i<mRow;++i)
	{
		float z=halfD-i*dz;
		for(UINT j=0;j<mCol;++j)
		{
			float x=-halfW+j*dx;
			float y=0.0f;
			mVertexs[i*mCol+j].pos=XMFLOAT3(x,y,z);
			mVertexs[i*mCol+j].normal=XMFLOAT3(0.0f,0.0f,0.0f);
			mVertexs[i*mCol+j].tex=XMFLOAT2(du*j,dv*i);
		}
	}
	D3D11_BUFFER_DESC vDesc={0};
	vDesc.ByteWidth=mNumVer*sizeof(Vertex::PosNormTex);
	vDesc.Usage=D3D11_USAGE_IMMUTABLE;
	vDesc.BindFlags=D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA vbdata={0};
	vbdata.pSysMem=&mVertexs[0];

	if(FAILED(mpd3dDevice->CreateBuffer(&vDesc,&vbdata,&mpVertexBuffer)))
		return 0;

	mIndices.resize(mNumFace*3);
	UINT k=0;
	for(UINT i=0;i<mCol-1;++i)
	{
		for(UINT j=0;j<mRow-1;++j)
		{
			mIndices[k]=i*mCol+j;
			mIndices[k+1]=i*mCol+j+1;
			mIndices[k+2]=(i+1)*mCol+j;

			mIndices[k+3]=(i+1)*mCol+j;
			mIndices[k+4]=i*mCol+j+1;
			mIndices[k+5]=(i+1)*mCol+j+1;
			k+=6;
		}
	}
	D3D11_BUFFER_DESC iDesc={0};
	iDesc.ByteWidth=mNumFace*3*sizeof(UINT);
	iDesc.Usage=D3D11_USAGE_IMMUTABLE;
	iDesc.BindFlags=D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA ibdata={0};
	ibdata.pSysMem=&mIndices[0];
	if(FAILED(mpd3dDevice->CreateBuffer(&iDesc,&ibdata,&mpIndexBuffer)))
		return 0;

	g_CaculateNormal(&mVertexs[0],mNumVer,&mIndices[0],mNumFace*3);

	

	return 1;
}
/*int Terrain::CreateShaderRV(LPCWSTR fileName)
{
     if(FAILED(D3DX11CreateShaderResourceViewFromFile(mpd3dDevice, fileName, 0, 0, &mShaderSView, 0 )))
	 {
		 return 0;
	 }
	 return 1;
}
*/
FullscreenQuad::FullscreenQuad():mpd3dDevice(NULL),mpVertexBuffer(NULL),mpIndexBuffer(NULL)
{
	
}
FullscreenQuad::~FullscreenQuad()
{
	ReleaseCOM(mpd3dDevice);
	ReleaseCOM(mpVertexBuffer);
	ReleaseCOM(mpIndexBuffer);
}
int FullscreenQuad::CreateBuffer(ID3D11Device* pd3dDevice)
{
	mpd3dDevice=pd3dDevice;

	mVertexs.resize(4);
	mIndices.resize(6);

	
	mVertexs[0].pos=XMFLOAT3(-1.0f, -1.0f, 0.0f);
	mVertexs[1].pos=XMFLOAT3(-1.0f, +1.0f, 0.0f);
	mVertexs[2].pos=XMFLOAT3(+1.0f, +1.0f, 0.0f);
	mVertexs[3].pos=XMFLOAT3(+1.0f, -1.0f, 0.0f);

	mVertexs[0].normal=XMFLOAT3(0.0f, 0.0f, -1.0f);
	mVertexs[1].normal=XMFLOAT3(0.0f, 0.0f, -1.0f);
	mVertexs[2].normal=XMFLOAT3(0.0f, 0.0f, -1.0f);
	mVertexs[3].normal=XMFLOAT3(0.0f, 0.0f, -1.0f);

	mVertexs[0].tex=XMFLOAT2(0.0f, 1.0f);
	mVertexs[1].tex=XMFLOAT2(0.0f, 0.0f);
	mVertexs[2].tex=XMFLOAT2(1.0f, 0.0f);
	mVertexs[3].tex=XMFLOAT2(1.0f, 1.0f);

	mIndices[0]=0;
	mIndices[1]=1;
	mIndices[2]=2;

	mIndices[3]=0;
	mIndices[4]=2;
	mIndices[5]=3;

	
    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex::PosNormTex) *4;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &mVertexs[0];

	if(FAILED(mpd3dDevice->CreateBuffer(&vbd,&vinitData,&mpVertexBuffer)))
		return 0;
	
	D3D11_BUFFER_DESC iDesc={0};
	iDesc.ByteWidth=6*sizeof(UINT);
	iDesc.Usage=D3D11_USAGE_IMMUTABLE;
	iDesc.BindFlags=D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA ibdata={0};
	ibdata.pSysMem=&mIndices[0];  
	if(FAILED(mpd3dDevice->CreateBuffer(&iDesc,&ibdata,&mpIndexBuffer)))
		return 0;
	return 1;
}
Sphere::Sphere():mpd3dDevice(NULL),mpVertexBuffer(NULL),
	            mpIndexBuffer(NULL),mRadius(0),mSliceCount(0),
				mStackCount(0)
{
	XMMATRIX I=::XMMatrixIdentity();
	::XMStoreFloat4x4(&mWorld,I);

}
Sphere::~Sphere()
{
	ReleaseCOM(mpd3dDevice);
	ReleaseCOM(mpVertexBuffer);
	ReleaseCOM(mpIndexBuffer);
}
void Sphere::SetWorld(float x,float y,float z)
{
	XMMATRIX W=XMMatrixTranslation(x,y,z );
	::XMStoreFloat4x4(&mWorld,W);
}
int Sphere::CreateBuffer(ID3D11Device* pd3dDevice,float radius)
{
	mpd3dDevice=pd3dDevice;
	mRadius=radius;

	mSliceCount=100;////////////////////////////
	mStackCount=100;

	Vertex::PosNormTex topVertex;
	Vertex::PosNormTex bottomVertex;

	topVertex.pos=XMFLOAT3(0.0f, +radius, 0.0f);
	topVertex.normal=XMFLOAT3(0.0f, +1.0f, 0.0f);
	topVertex.tex=XMFLOAT2( 0.0f, 0.0f);

	bottomVertex.pos=XMFLOAT3(0.0f, -radius, 0.0f);
	bottomVertex.normal=XMFLOAT3(0.0f, -1.0f, 0.0f);
	bottomVertex.tex=XMFLOAT2( 0.0f, 1.0f);

	mVertexs.push_back(topVertex);

	float phiStep   = XM_PI/mStackCount;
	float thetaStep = 2.0f*XM_PI/mSliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for(UINT i = 1; i <= mStackCount-1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
		for(UINT j = 0; j <= mSliceCount; ++j)
		{
			float theta = j*thetaStep;

			Vertex::PosNormTex v;

			// spherical to cartesian
			v.pos.x = radius*sinf(phi)*cosf(theta);
			v.pos.y = radius*cosf(phi);
			v.pos.z = radius*sinf(phi)*sinf(theta);

			

			XMVECTOR p = XMLoadFloat3(&v.pos);
			XMStoreFloat3(&v.normal, XMVector3Normalize(p));

			v.tex.x = theta / XM_2PI;
			v.tex.y = phi / XM_PI;

			mVertexs.push_back( v );
		}
	}

	mVertexs.push_back( bottomVertex );

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for(UINT i = 1; i <= mSliceCount; ++i)
	{
		mIndices.push_back(0);
		mIndices.push_back(i+1);
		mIndices.push_back(i);
	}
	
	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = mSliceCount+1;
	for(UINT i = 0; i < mStackCount-2; ++i)
	{
		for(UINT j = 0; j < mSliceCount; ++j)
		{
			mIndices.push_back(baseIndex + i*ringVertexCount + j);
			mIndices.push_back(baseIndex + i*ringVertexCount + j+1);
			mIndices.push_back(baseIndex + (i+1)*ringVertexCount + j);

			mIndices.push_back(baseIndex + (i+1)*ringVertexCount + j);
			mIndices.push_back(baseIndex + i*ringVertexCount + j+1);
			mIndices.push_back(baseIndex + (i+1)*ringVertexCount + j+1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)mVertexs.size()-1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;
	
	for(UINT i = 0; i < mSliceCount; ++i)
	{
		mIndices.push_back(southPoleIndex);
		mIndices.push_back(baseIndex+i);
		mIndices.push_back(baseIndex+i+1);
	}
//
//
	 D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::PosNormTex) *mVertexs.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &mVertexs[0];

	if(FAILED(mpd3dDevice->CreateBuffer(&vbd,&vinitData,&mpVertexBuffer)))
		return 0;
	
	D3D11_BUFFER_DESC iDesc={0};
	iDesc.ByteWidth=mIndices.size()*sizeof(UINT);
	iDesc.Usage=D3D11_USAGE_IMMUTABLE;
	iDesc.BindFlags=D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA ibdata={0};
	ibdata.pSysMem=&mIndices[0];  
	if(FAILED(mpd3dDevice->CreateBuffer(&iDesc,&ibdata,&mpIndexBuffer)))
		return 0;
	return 1;
}