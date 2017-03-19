#include"Light.hlsl"
#include"Blend.hlsl"

cbuffer cbPerFrame
{
   DirectionalLight gDirLight[3];
   float3           gEyePosW;

};
cbuffer cbPerObject
{
   float4x4 gWorld;
   float4x4 gWorldInvTranspose;
   float4x4 gWorldViewProj;
   float4x4 gTexTransform;
   Material gMaterial;
};
// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct VertexIn
{
   float3 PosL     :POSITION;
   float3 NormalL  :NORMAL;
   float2 Tex      : TEXCOORD;
};
struct VertexOut
{
   float3 PosW     :POSITION;
   float4 PosH      :SV_POSITION;
   float3 NormalW   :NORMAL;
   float2 Tex       : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
   VertexOut vout;
   vout.PosW=mul(float4(vin.PosL,1.0f),gWorld).xyz;
   vout.PosH=mul(float4(vin.PosL,1.0f),gWorldViewProj);
   vout.NormalW=mul(vin.NormalL,(float3x3)gWorldInvTranspose);
   vout.Tex=mul(float4(vin.Tex,0.0f,1.0f),gTexTransform).xy;
   return vout;
}
float4 PS(VertexOut pin,
	uniform bool gAlphaClip) :SV_Target
{
   pin.NormalW = normalize(pin.NormalW);

   float4 ambient=float4(0.0f,0.0f,0.0f,0.0f);
   float4 diffuse=float4(0.0f,0.0f,0.0f,0.0f);
   float4 spec   =float4(0.0f,0.0f,0.0f,0.0f);

   float3 toEyeW=gEyePosW-pin.PosW;
   float  distToEye=length(toEyeW);
   toEyeW=toEyeW/distToEye;

   float4 texColor=float4(1.0f,1.0f,1.0f,1.0f);

   texColor = gDiffuseMap.Sample( samAnisotropic, pin.Tex );
   if (gAlphaClip)
   {
	   clip(texColor.a - 0.1f);//if((texColor.a-0.1f)<0)Å×Æú¸ÃÏñËØ
   }

//   [unroll]//--------------------------------------------

   float4 A,D,S;
   for(int i=0;i<3;++i)
   {
   ComputeDirectionalLight(gMaterial, gDirLight[i], pin.NormalW, toEyeW, A, D, S);
   ambient += A;  
   diffuse += D;
   spec    += S; 
   }
   float4 litColor=texColor*(ambient+diffuse)+spec;  

    litColor.a=gMaterial.Diffuse.a*texColor.a;

	return litColor; 
}
technique11 Tech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(false) ) );
    }

}
technique11 TechAlphaClip
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(true)));
	}

}
