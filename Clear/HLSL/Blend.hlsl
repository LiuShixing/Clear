
BlendState blend
{
//Blending state for first render target.
   BlendEnable[0]=TRUE;
   SrcBlend[0]=SRC_ALPHA;       //设置
   DestBlend[0]=INV_SRC_ALPHA;
   BlendOp[0]=ADD;
   SrcBlendAlpha[0]=SRC_ALPHA;
   DestBlendAlpha[0]=INV_SRC_ALPHA;
   BlendOpAlpha[0]=ADD;
   RenderTargetWriteMask[0]=0x0F;//融合后写入全部通道

//Blending state for second simultaneous(同时的） render target.
   BlendEnable[1]=True;
   SrcBlend[1]=One;
   DestBlend[1]=Zero;
   BlendOp[1]=Add;
   SrcBlendAlpha[1]=Zero;
   DestBlendAlpha[1]=Zero;
   BlendOpAlpha[1]=Add;
   RenderTargetWriteMask[1]=0x0F;
};


struct Fog
{
   int    IsFog;
   int     Num;
   float   FogStart;
   float   FogRange;
   float4  FogColor;
};
void ComputeFog(Fog fog,float distToEye,inout float4 Color)//Color 即为输入也为输出
{
//    if(fog.Num==0)
//	{
//        fog.FogStart=5.0f;
 //   	fog.FogRange=500.0f;
//	    fog.FogColor=float4(0.75f,0.75f,0.75f,1.0f);
//	    fog.Num=3;
//	}

    float flogLerp=saturate((distToEye-fog.FogStart)/fog.FogRange);
	for(int i=0;i<fog.Num;++i)
	{
    	Color=lerp(Color,fog.FogColor,flogLerp);
	}


}