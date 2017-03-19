cbuffer PerFrame
{
  float4x4 gWorldViewProj;
  float4x4 gWorld;
  float4x4 gWorldInvTranspose;
};
struct VertexIn
{
   float3 pos    :POSITION;
   float3 normal  :NORMAL;
   float2 tex     :TEXCOORD;
   float4 tangent  :TANGENT;
};
struct VertexOut
{
   float3 posW     :POSITION;
   float4 posH   :SV_POSITION;
   float3 normalW  :NORMAL;
   float2 tex     :TEXCOORD;
   float4 tangent  :TANGENT;
};
VertexOut VS(VertexIn vin)
{
   VertexOut vout;
   vout.posH=mul(float4(vin.pos,1.0f),gWorldViewProj);
   vout.posW = mul(float4(vin.pos, 1.0f), gWorld).xyz;
   vout.normalW = mul(vin.normal, (float3x3)gWorldInvTranspose);
   return vout;
}
float4 PS(VertexOut pin) : SV_Target
{
   return float4(pin.normalW,1.0f);
//	return float4(0.0f,1.0f,0.0f,1.0f);
}
technique11 BasicDraw
{
   pass PO
   {
      SetVertexShader(CompileShader(vs_5_0,VS()));
	  SetPixelShader(CompileShader(ps_5_0,PS()));
   }
}