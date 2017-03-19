#include"Effect.h"
#include"Geometry.h"

int g_KeyDown(int vKey)
{
    return GetAsyncKeyState(vKey) & 0x8000;
}


void DirectLight::SetColor(UINT color,float intensity)
{
	g_SetColor(color,intensity,Ambient,Diffuse,Specular);
}
void PointLight::SetColor(UINT color,float intensity)
{
	g_SetColor(color,intensity,Ambient,Diffuse,Specular);
}
void SpotLight::SetColor(UINT color,float intensity)
{
	g_SetColor(color,intensity,Ambient,Diffuse,Specular);
}
void Material::SetMat(UINT color,float intensity)
{
	g_SetColor(color,intensity,Ambient,Diffuse,Specular);
	if(color!=SKYBLUE)
	{
    	Specular.w=16.0f;
    	Diffuse.w=0.5f;
	}
}
void g_SetColor(UINT color,float intensity,XMFLOAT4& A,XMFLOAT4& D,XMFLOAT4& S)
{
	switch(color)
	{
	case WHITE://White
		A =	XMFLOAT4(1.0f*intensity, 1.0f*intensity, 1.0f*intensity, 1.0f);
        D=	XMFLOAT4(1.0f*intensity, 1.0f*intensity, 1.0f*intensity, 1.0f);
        S=	XMFLOAT4(1.0f*intensity, 1.0f*intensity, 1.0f*intensity, 1.0f);
		break;
	case BLACK:
		A =	XMFLOAT4(1.0f-intensity,1.0f-intensity, 1.0f-intensity, 1.0f);
        D=	XMFLOAT4(1.0f-intensity,1.0f-intensity, 1.0f-intensity, 1.0f);
        S=	XMFLOAT4(1.0f-intensity,1.0f-intensity, 1.0f-intensity, 1.0f);
		break;
	case RED:
		A =	XMFLOAT4(1.0f*intensity, 0.0f, 0.0f, 1.0f);
        D=	XMFLOAT4(1.0f*intensity, 0.0f, 0.0f, 1.0f);
        S=	XMFLOAT4(1.0f*intensity, 0.0f, 0.0f, 1.0f);
		break;
	case GREEN :
		A =	XMFLOAT4(0.0f, 1.0f*intensity, 0.0f, 1.0f);
        D=	XMFLOAT4(0.0f, 1.0f*intensity, 0.0f, 1.0f);
        S=	XMFLOAT4(0.0f, 1.0f*intensity, 0.0f, 1.0f);
		break;
	case BLUE:
		A =	XMFLOAT4(0.0f, 0.0f, 1.0f*intensity, 1.0f);
        D=	XMFLOAT4(0.0f, 0.0f, 1.0f*intensity, 1.0f);
        S=	XMFLOAT4(0.0f, 0.0f, 1.0f*intensity, 1.0f);
		break;
	case YELLOW:
		A =	XMFLOAT4(1.0f*intensity, 1.0f*intensity, 0.0f, 1.0f);
        D=	XMFLOAT4(1.0f*intensity, 1.0f*intensity, 0.0f, 1.0f);
        S=	XMFLOAT4(1.0f*intensity, 1.0f*intensity, 0.0f, 1.0f);
		break;
	case CYAN:
		A =	XMFLOAT4(0.0f, 1.0f*intensity, 1.0f*intensity, 1.0f);
        D=	XMFLOAT4(0.0f, 1.0f*intensity, 1.0f*intensity, 1.0f);
        S=	XMFLOAT4(0.0f, 1.0f*intensity, 1.0f*intensity, 1.0f);
		break;
	case MAGENTA:
		A =	XMFLOAT4(1.0f*intensity, 0.0f, 1.0f*intensity, 1.0f);
        D=	XMFLOAT4(1.0f*intensity, 0.0f, 1.0f*intensity, 1.0f);
        S=	XMFLOAT4(1.0f*intensity, 0.0f, 1.0f*intensity, 1.0f);
		break;
	case SILVER:
		A =	XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f);
        D=	XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f);
        S=	XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f);
		break;
	case LIGHT_STEEL_BLUE:
		A =	XMFLOAT4(0.69f, 0.77f, 0.87f, 1.0f);
        D=	XMFLOAT4(0.69f, 0.77f, 0.87f, 1.0f);
        S=	XMFLOAT4(0.69f, 0.77f, 0.87f, 1.0f);
		break;
	case SKYBLUE:
		A =	XMFLOAT4 (0.5f*intensity, 0.5f*intensity, 0.5f*intensity, 1.0f);
    	D =	XMFLOAT4(1.0f*intensity, 1.0f*intensity, 1.0f*intensity, 0.5f);
    	S =	XMFLOAT4(0.8f*intensity, 0.8f*intensity, 0.8f*intensity, 100.0f);
		break;
	}
}



ID3D11BlendState*      Blend::mTransparentBS=0;
ID3D11BlendState*      Blend::mNoColorWBS=0;

int Blend::InitAll(ID3D11Device*   pd3dDevice)
{

	D3D11_BLEND_DESC       TransparentDesc;
	D3D11_BLEND_DESC       NoColorWriteDesc;

	TransparentDesc.AlphaToCoverageEnable=false;
	TransparentDesc.IndependentBlendEnable=false;//不针对多个RenderTarget使用不同的混合状态
	//因此只设置第一个数组元素即可 
	TransparentDesc.RenderTarget[0].BlendEnable=true;
	TransparentDesc.RenderTarget[0].SrcBlend=D3D11_BLEND_SRC_ALPHA;
	TransparentDesc.RenderTarget[0].DestBlend=D3D11_BLEND_INV_SRC_ALPHA;
	TransparentDesc.RenderTarget[0].BlendOp=D3D11_BLEND_OP_ADD;
	TransparentDesc.RenderTarget[0].SrcBlendAlpha=D3D11_BLEND_ONE;
	TransparentDesc.RenderTarget[0].DestBlendAlpha=D3D11_BLEND_ZERO;
	TransparentDesc.RenderTarget[0].BlendOpAlpha=D3D11_BLEND_OP_ADD;
	TransparentDesc.RenderTarget[0].RenderTargetWriteMask=D3D11_COLOR_WRITE_ENABLE_ALL;

	
	NoColorWriteDesc.AlphaToCoverageEnable=false;
	NoColorWriteDesc.IndependentBlendEnable=false;//不针对多个RenderTarget使用不同的混合状态
	//因此只设置第一个数组元素即可 
	NoColorWriteDesc.RenderTarget[0].BlendEnable=true;
	NoColorWriteDesc.RenderTarget[0].SrcBlend=D3D11_BLEND_SRC_ALPHA;
	NoColorWriteDesc.RenderTarget[0].DestBlend=D3D11_BLEND_INV_SRC_ALPHA;
	NoColorWriteDesc.RenderTarget[0].BlendOp=D3D11_BLEND_OP_ADD;
	NoColorWriteDesc.RenderTarget[0].SrcBlendAlpha=D3D11_BLEND_ONE;
	NoColorWriteDesc.RenderTarget[0].DestBlendAlpha=D3D11_BLEND_ZERO;
	NoColorWriteDesc.RenderTarget[0].BlendOpAlpha=D3D11_BLEND_OP_ADD;
	NoColorWriteDesc.RenderTarget[0].RenderTargetWriteMask=0;//disable color writes to the back buffer

	if(FAILED(pd3dDevice->CreateBlendState(&TransparentDesc,&mTransparentBS)))
	{
		return 0;
	}
	if(FAILED(pd3dDevice->CreateBlendState(&NoColorWriteDesc,&mNoColorWBS)))
	{
		return 0;
	}

	return 1;
}

ID3D11DepthStencilState*     Stencil::mNoDepthButStencilSS=0;
ID3D11DepthStencilState*     Stencil::mDrawReflectSS=0;
ID3D11DepthStencilState*     Stencil::mNoDoubleBlendSS=0;

int Stencil::InitAll(ID3D11Device*   d3dDevice)
{

	D3D11_DEPTH_STENCIL_DESC     NoWriteDepthButStencilDesc;
	D3D11_DEPTH_STENCIL_DESC     DrawReflectionDesc;
	D3D11_DEPTH_STENCIL_DESC     NoDoubleBlendDesc;

	NoWriteDepthButStencilDesc.DepthEnable=true;
	NoWriteDepthButStencilDesc.DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ZERO;//////////////
	NoWriteDepthButStencilDesc.DepthFunc=D3D11_COMPARISON_LESS;
	NoWriteDepthButStencilDesc.StencilEnable=true;//
	NoWriteDepthButStencilDesc.StencilReadMask=0xff;
	NoWriteDepthButStencilDesc.StencilWriteMask=0xff;
	NoWriteDepthButStencilDesc.FrontFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;
	NoWriteDepthButStencilDesc.FrontFace.StencilDepthFailOp=D3D11_STENCIL_OP_KEEP;
	NoWriteDepthButStencilDesc.FrontFace.StencilPassOp=D3D11_STENCIL_OP_REPLACE;
	NoWriteDepthButStencilDesc.FrontFace.StencilFunc=D3D11_COMPARISON_ALWAYS;

	NoWriteDepthButStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	NoWriteDepthButStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
    NoWriteDepthButStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	NoWriteDepthButStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//------------------------------------------------------------------------------------------
	DrawReflectionDesc.DepthEnable=true;
	DrawReflectionDesc.DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ALL;//
	DrawReflectionDesc.DepthFunc=D3D11_COMPARISON_LESS;
	DrawReflectionDesc.StencilEnable=true;
    DrawReflectionDesc.StencilReadMask=0xff;
	DrawReflectionDesc.StencilWriteMask=0xff;
	DrawReflectionDesc.FrontFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;
	DrawReflectionDesc.FrontFace.StencilDepthFailOp=D3D11_STENCIL_OP_KEEP;
	DrawReflectionDesc.FrontFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;//
	DrawReflectionDesc.FrontFace.StencilFunc=D3D11_COMPARISON_EQUAL;//

	DrawReflectionDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	DrawReflectionDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    DrawReflectionDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	DrawReflectionDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//-----------------------------------------------------------------------------------------
	
	NoDoubleBlendDesc.DepthEnable=true;
	NoDoubleBlendDesc.DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ALL;//
	NoDoubleBlendDesc.DepthFunc=D3D11_COMPARISON_LESS;
	NoDoubleBlendDesc.StencilEnable=true;
	NoDoubleBlendDesc.StencilReadMask=0xff;
	NoDoubleBlendDesc.StencilWriteMask=0xff;
	NoDoubleBlendDesc.FrontFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;
	NoDoubleBlendDesc.FrontFace.StencilDepthFailOp=D3D11_STENCIL_OP_KEEP;
	NoDoubleBlendDesc.FrontFace.StencilPassOp=D3D11_STENCIL_OP_INCR;//
	NoDoubleBlendDesc.FrontFace.StencilFunc=D3D11_COMPARISON_EQUAL;//

	NoDoubleBlendDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	NoDoubleBlendDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
    NoDoubleBlendDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	NoDoubleBlendDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

	if(FAILED(d3dDevice->CreateDepthStencilState(&NoWriteDepthButStencilDesc,&mNoDepthButStencilSS)))
	{
		return 0;
	}
	if(FAILED(d3dDevice->CreateDepthStencilState(&DrawReflectionDesc,&mDrawReflectSS)))
	{
		return 0;
	}
	if(FAILED(d3dDevice->CreateDepthStencilState(&NoDoubleBlendDesc,&mNoDoubleBlendSS)))
	{
		return 0;
	}
	return 1;
}

ID3D11RasterizerState*   RasterizerState::mNoBackRS=0;
ID3D11RasterizerState*   RasterizerState::mWireframeRS=0;
ID3D11RasterizerState*   RasterizerState::mCounterClockFrontRS=0;

int RasterizerState::InitAll(ID3D11Device* device)
{
	D3D11_RASTERIZER_DESC    NoBackDesc;
	D3D11_RASTERIZER_DESC    DefualtDesc;
	D3D11_RASTERIZER_DESC    WireframeDesc;
	D3D11_RASTERIZER_DESC    CounterClockFrontDesc;

	ZeroMemory(&DefualtDesc,sizeof(D3D11_RASTERIZER_DESC));
	DefualtDesc.CullMode=D3D11_CULL_BACK;
	DefualtDesc.FillMode=D3D11_FILL_SOLID;
	DefualtDesc.DepthClipEnable=true;

	ZeroMemory(&NoBackDesc,sizeof(D3D11_RASTERIZER_DESC));
	NoBackDesc.CullMode=D3D11_CULL_NONE;
	NoBackDesc.FillMode=D3D11_FILL_SOLID;
	NoBackDesc.DepthClipEnable=true;

	ZeroMemory(&WireframeDesc,sizeof(D3D11_RASTERIZER_DESC));
	WireframeDesc.CullMode=D3D11_CULL_BACK;
	WireframeDesc.FillMode=D3D11_FILL_WIREFRAME;
	WireframeDesc.DepthClipEnable=true;

	ZeroMemory(&CounterClockFrontDesc,sizeof(D3D11_RASTERIZER_DESC));
	CounterClockFrontDesc.CullMode=D3D11_CULL_BACK;
	CounterClockFrontDesc.FillMode=D3D11_FILL_SOLID;
	CounterClockFrontDesc.DepthClipEnable=true;
	CounterClockFrontDesc.FrontCounterClockwise=true;

	if(FAILED(device->CreateRasterizerState(&NoBackDesc,&mNoBackRS)))
		return 0;
	if(FAILED(device->CreateRasterizerState(&WireframeDesc,&mWireframeRS)))
		return 0;
	if(FAILED(device->CreateRasterizerState(&CounterClockFrontDesc,&mCounterClockFrontRS)))
		return 0;
	return 1;
}





Blur::Blur():mOffscreenSRV(NULL),mOffscreenRTV(NULL),mOffscreenUAV(NULL),
	        mBlurredTexSRV(NULL),mBlurredTexUAV(NULL)
{

}
Blur::~Blur()
{

}

int Blur::CreateSomeView(ID3D11Device* d3dDevice,UINT ClientWidth,UINT ClientHeight)
{
	md3dDevice=d3dDevice;
	mClientWidth=ClientWidth;
	mClientHeight=ClientHeight;

	// We call this function everytime the window is resized so that the render target is a quarter
	// the client area dimensions.  So Release the previous views before we create new ones.
	ReleaseCOM(mOffscreenSRV);  //执行前mOffscreenSRV 必须初始化
	ReleaseCOM(mOffscreenRTV);
	ReleaseCOM(mOffscreenUAV);

	D3D11_TEXTURE2D_DESC texDesc;
	
	texDesc.Width     = mClientWidth;
	texDesc.Height    = mClientHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format    = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count   = 1;  
	texDesc.SampleDesc.Quality = 0;  
	texDesc.Usage          = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags      = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0; 
	texDesc.MiscFlags      = 0;

	ID3D11Texture2D* offscreenTex = 0;
	HR(md3dDevice->CreateTexture2D(&texDesc, 0, &offscreenTex));
	
	// Null description means to create a view to all mipmap levels using 
	// the format the texture was created with.
	if(FAILED(md3dDevice->CreateShaderResourceView(offscreenTex, 0, &mOffscreenSRV)))
		return 0;
	if(FAILED(md3dDevice->CreateRenderTargetView(offscreenTex, 0, &mOffscreenRTV)))
		return 0;
	if(FAILED(md3dDevice->CreateUnorderedAccessView(offscreenTex, 0, &mOffscreenUAV)))
		return 0;

	// View saves a reference to the texture so we can release our reference.
	ReleaseCOM(offscreenTex);



// Create BlurTexView

	// Start fresh.
	ReleaseCOM(mBlurredTexSRV);
	ReleaseCOM(mBlurredTexUAV);

	// Note, compressed formats cannot be used for UAV.  We get error like:
	// ERROR: ID3D11Device::CreateTexture2D: The format (0x4d, BC3_UNORM) 
	// cannot be bound as an UnorderedAccessView, or cast to a format that
	// could be bound as an UnorderedAccessView.  Therefore this format 
	// does not support D3D11_BIND_UNORDERED_ACCESS.

	D3D11_TEXTURE2D_DESC blurredTexDesc;
	blurredTexDesc.Width     = mClientWidth;
	blurredTexDesc.Height    = mClientHeight;
    blurredTexDesc.MipLevels = 1;
    blurredTexDesc.ArraySize = 1;
	blurredTexDesc.Format    = DXGI_FORMAT_R8G8B8A8_UNORM;
	blurredTexDesc.SampleDesc.Count   = 1;
	blurredTexDesc.SampleDesc.Quality = 0;
    blurredTexDesc.Usage     = D3D11_USAGE_DEFAULT;
    blurredTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    blurredTexDesc.CPUAccessFlags = 0;
    blurredTexDesc.MiscFlags      = 0;

	ID3D11Texture2D* blurredTex = 0;
	if(FAILED(md3dDevice->CreateTexture2D(&blurredTexDesc, 0, &blurredTex)))
		return 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	if(FAILED(md3dDevice->CreateShaderResourceView(blurredTex, &srvDesc, &mBlurredTexSRV)))
		return 0;

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format =DXGI_FORMAT_R8G8B8A8_UNORM;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;
	if(FAILED(md3dDevice->CreateUnorderedAccessView(blurredTex, &uavDesc, &mBlurredTexUAV)))
		return 0;

	// Views save a reference to the texture so we can release our reference.
	ReleaseCOM(blurredTex);

	return 1;
}
int Blur::BuildFX(LPCWSTR effectFileName)
{
	  UINT flag(0);  
#if defined(DEBUG) || defined(_DEBUG)  
    flag |= D3D10_SHADER_DEBUG;  
    flag |= D3D10_SHADER_SKIP_OPTIMIZATION;  
#endif  
	ID3D10Blob* shader(NULL);
	ID3D10Blob* errMSG(NULL);
	HRESULT hr=D3DX11CompileFromFile(effectFileName,0,0,0,"fx_5_0",flag,0,0,&shader,&errMSG,0);
	if(errMSG)  
    {  
       MessageBoxA(NULL,(char*)errMSG->GetBufferPointer(),"ShaderCompileError",MB_OK);  
       errMSG->Release();  
       return 0;  
    }  
    if(FAILED(hr))  
    {  
       MessageBox(NULL,L"CompileShader错误!",L"错误",MB_OK);  
       return 0;  
    }  
	if(FAILED(::D3DX11CreateEffectFromMemory(shader->GetBufferPointer(),shader->GetBufferSize(),0,md3dDevice,&mBlurFX)))
	{
		MessageBox(NULL,L"D3DX11CreateEffectFromMemory错误!",L"错误",MB_OK);  
        return 0;  
	}
	ReleaseCOM(shader);

	mBlurFxInput=mBlurFX->GetVariableByName("gInput")->AsShaderResource();
	mBlurFxOutput=mBlurFX->GetVariableByName("gOutput")->AsUnorderedAccessView();

	mHorzBlurTech=mBlurFX->GetTechniqueByName("HorzBlur");
	mVertBlurTech=mBlurFX->GetTechniqueByName("VertBlur");

	return 1;
}
void  Blur::DoBlur(ID3D11DeviceContext* dc,UINT blurCount)
{

	for(UINT i=0;i<blurCount;++i)
	{
    	D3DX11_TECHNIQUE_DESC techDesc;               
    	mHorzBlurTech->GetDesc(&techDesc);
    	for(UINT p=0;p<techDesc.Passes;++p)
    	{
	    	mBlurFxInput->SetResource(mOffscreenSRV);
	    	mBlurFxOutput->SetUnorderedAccessView(mBlurredTexUAV);
	    	mHorzBlurTech->GetPassByIndex(p)->Apply(0,dc);

    		UINT GroupX=(UINT)ceilf(this->mClientWidth/256.0f);
        	dc->Dispatch(GroupX, this->mClientHeight, 1);     //分配线程 并计算
		
         }

	// Unbind the input texture from the CS for good housekeeping.
    	ID3D11ShaderResourceView* nullSRV[1] = { 0 };
    	dc->CSSetShaderResources( 0, 1, nullSRV );

		// Unbind output from compute shader (we are going to use this output as an input in the next pass, 
  		// and a resource cannot be both an output and input at the same time.
    	ID3D11UnorderedAccessView* nullUAV[1] = { 0 };
     	dc->CSSetUnorderedAccessViews( 0, 1, nullUAV, 0 );

	    mVertBlurTech->GetDesc(&techDesc);
    	for(UINT p=0;p<techDesc.Passes;++p)
    	{
    		mBlurFxInput->SetResource(mBlurredTexSRV);
    		mBlurFxOutput->SetUnorderedAccessView( mOffscreenUAV);
    		mVertBlurTech->GetPassByIndex(p)->Apply(0,dc);

    		UINT GroupY=(UINT)ceilf(this->mClientHeight/256.0f);
    		dc->Dispatch(this->mClientWidth, GroupY, 1);     //分配线程 并计算
    	
         }

    	dc->CSSetShaderResources( 0, 1, nullSRV );
    	dc->CSSetUnorderedAccessViews( 0, 1, nullUAV, 0 );

	
	}
	// Disable compute shader.
	dc->CSSetShader(0, 0, 0);
}

Sky::Sky()
{
}
Sky::~Sky()
{
}

int Sky::Init(ID3D11Device* d3dDevice, const std::wstring& cubemapFilename, const std::wstring& hlslFileName)
{
	if(!CreateBuffer(d3dDevice))
		return 0;
	if (!BuildFX(d3dDevice, hlslFileName))
		return 0;
	if(FAILED(D3DX11CreateShaderResourceViewFromFile(d3dDevice,cubemapFilename.c_str(),0,0,&mCubeMapSRV,0)))
		return 0;
	if(!InputLayout::InitPos(d3dDevice,mTech))
	{
		MessageBox(0, L"InputLayout::InitPos failed", 0, 0);
		return 0;
	}
	return 1;
}
void Sky::Draw(ID3D11DeviceContext* dc,Camera& camera)
{
	dc->IASetInputLayout(InputLayout::mPos);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride =sizeof(XMFLOAT3);
	UINT offset = 0;

	dc->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	dc->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	// center Sky about eye in world space
	XMFLOAT3 eyePos = camera.GetPosition();
	XMMATRIX WORLD=XMMatrixTranslation(eyePos.x, eyePos.y, eyePos.z);
	XMMATRIX WorldViewProj=WORLD*camera.GetView()*camera.GetProj();
	
	mFxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&WorldViewProj));
	mFxCubeMapSRV->SetResource(mCubeMapSRV);

	D3DX11_TECHNIQUE_DESC techDesc;               
	mTech->GetDesc(&techDesc);
	for(UINT p=0;p<techDesc.Passes;++p)
	{
		mTech->GetPassByIndex(p)->Apply(0,dc);
		dc->DrawIndexed( mIndiceCount,0,0);
     }
	// restore default states, as the SkyFX changes them in the effect file.
	dc->RSSetState(0);
	dc->OMSetDepthStencilState(0, 0);
}
int Sky::CreateBuffer(ID3D11Device* d3dDevice)
{
		float radius=5000.0f;

	UINT SliceCount=100;////////////////////////////
	UINT StackCount=100;

	XMFLOAT3 topVertex;
	XMFLOAT3 bottomVertex;

	topVertex=XMFLOAT3(0.0f, +radius, 0.0f);

	bottomVertex=XMFLOAT3(0.0f, -radius, 0.0f);
	

	std::vector<XMFLOAT3> Vertexs;

	Vertexs.push_back(topVertex);

	float phiStep   = XM_PI/StackCount;
	float thetaStep = 2.0f*XM_PI/SliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for(UINT i = 1; i <= StackCount-1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
		for(UINT j = 0; j <= SliceCount; ++j)
		{
			float theta = j*thetaStep;

			XMFLOAT3 v;

			// spherical to cartesian
			v.x = radius*sinf(phi)*cosf(theta);
			v.y = radius*cosf(phi);
			v.z = radius*sinf(phi)*sinf(theta);

			Vertexs.push_back( v );
		}
	}

	Vertexs.push_back( bottomVertex );

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//
	std::vector<UINT> Indices;
	for(UINT i = 1; i <= SliceCount; ++i)
	{
		Indices.push_back(0);
		Indices.push_back(i+1);
		Indices.push_back(i);
	}
	
	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = SliceCount+1;
	for(UINT i = 0; i < StackCount-2; ++i)
	{
		for(UINT j = 0; j < SliceCount; ++j)
		{
			Indices.push_back(baseIndex + i*ringVertexCount + j);
			Indices.push_back(baseIndex + i*ringVertexCount + j+1);
			Indices.push_back(baseIndex + (i+1)*ringVertexCount + j);

			Indices.push_back(baseIndex + (i+1)*ringVertexCount + j);
			Indices.push_back(baseIndex + i*ringVertexCount + j+1);
			Indices.push_back(baseIndex + (i+1)*ringVertexCount + j+1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)Vertexs.size()-1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;
	
	for(UINT i = 0; i < SliceCount; ++i)
	{
		Indices.push_back(southPoleIndex);
		Indices.push_back(baseIndex+i);
		Indices.push_back(baseIndex+i+1);
	}
//
//
	mVertexCount=Vertexs.size();

	 D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(XMFLOAT3) *mVertexCount;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &Vertexs[0];

	if(FAILED(d3dDevice->CreateBuffer(&vbd,&vinitData,&mVB)))
		return 0;
	
	mIndiceCount=Indices.size();

	D3D11_BUFFER_DESC iDesc={0};
	iDesc.ByteWidth=sizeof(UINT)*mIndiceCount;
	iDesc.Usage=D3D11_USAGE_IMMUTABLE;
	iDesc.BindFlags=D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA ibdata={0};
	ibdata.pSysMem=&Indices[0];  
	if(FAILED(d3dDevice->CreateBuffer(&iDesc,&ibdata,&mIB)))
		return 0;
	return 1;
}
int Sky::BuildFX(ID3D11Device* d3dDevice, const std::wstring& hlslFileName)
{
		  UINT flag(0);  
#if defined(DEBUG) || defined(_DEBUG)  
    flag |= D3D10_SHADER_DEBUG;  
    flag |= D3D10_SHADER_SKIP_OPTIMIZATION;  
#endif  
	ID3D10Blob* shader(NULL);
	ID3D10Blob* errMSG(NULL);
	HRESULT hr = D3DX11CompileFromFile(hlslFileName.c_str(), 0, 0, 0, "fx_5_0", flag, 0, 0, &shader, &errMSG, 0);
	if(errMSG)  
    {  
       MessageBoxA(NULL,(char*)errMSG->GetBufferPointer(),"ShaderCompileError",MB_OK);  
       errMSG->Release();  
       return 0;  
    }  
    if(FAILED(hr))  
    {  
       MessageBox(NULL,L"CompileShader错误!",L"错误",MB_OK);  
       return 0;  
    }  
	if(FAILED(::D3DX11CreateEffectFromMemory(shader->GetBufferPointer(),shader->GetBufferSize(),0,d3dDevice,&mFX)))
	{
		MessageBox(NULL,L"D3DX11CreateEffectFromMemory错误!",L"错误",MB_OK);  
        return 0;  
	}
	ReleaseCOM(shader);

	
	mFxWorldViewProj=mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	mTech=mFX->GetTechniqueByName("SkyTech");
	mFxCubeMapSRV=mFX->GetVariableByName("gCubeMap")->AsShaderResource();

	return 1;
}


ID3DX11Effect* Effect::mFX=NULL;
int Effect::BuildFX(ID3D11Device* d3dDevice,const std::wstring& filename)
{
		  UINT flag(0);  
#if defined(DEBUG) || defined(_DEBUG)  
    flag |= D3D10_SHADER_DEBUG;  
    flag |= D3D10_SHADER_SKIP_OPTIMIZATION;  
#endif  
	ID3D10Blob* shader(NULL);
	ID3D10Blob* errMSG(NULL);
	HRESULT hr;
//---------------------------------------------------------------------------------------

	hr=D3DX11CompileFromFile(filename.c_str(),0,0,0,"fx_5_0",flag,0,0,&shader,&errMSG,0);
	if(errMSG)  
    {  
       MessageBoxA(NULL,(char*)errMSG->GetBufferPointer(),"ShaderCompileError",MB_OK);  
       errMSG->Release();  
       return 0;  
    }  
    if(FAILED(hr))  
    {  
       MessageBox(NULL,L"CompileShader错误!",L"错误",MB_OK);  
       return 0;  
    }  
	if(FAILED(::D3DX11CreateEffectFromMemory(shader->GetBufferPointer(),shader->GetBufferSize(),0,d3dDevice,&mFX)))
	{
		MessageBox(NULL,L"D3DX11CreateEffectFromMemory错误!",L"错误",MB_OK);  
        return 0;  
	}
	ReleaseCOM(shader);

	return 1;
}