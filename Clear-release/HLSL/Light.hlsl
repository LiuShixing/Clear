struct DirectionalLight
{
   float4 Ambient;
   float4 Diffuse;
   float4 Specular;
   float3 Direction;
   float  pad;//用来凑成4维
};
struct PointLight
{
   float4 Ambient;
   float4 Diffuse;
   float4 Specular;

   float3 Position;
   float  Range;//范围

   float3 Att;//衰减
   float  pad;
};
struct SpotLight
{
   float4 Ambient;
   float4 Diffuse;
   float4 Specular;

   float3 Position;
   float  Range;

   float3 Direction;
   float  Spot;//控制圆锥

   float3 Att;
   float  pad;
};
struct Material
{
   float4 Ambient;
   float4 Diffuse;
   float4 Specular;//w=SpecPower
   float4 Reflect;
};

void ComputeDirectionalLight(Material mat,DirectionalLight light,float3 normal,float3 toEye,
                             out float4 ambient,out float4 diffuse,out float4 specular)
{
   ambient=float4(0.0f,0.0f,0.0f,0.0f);
   diffuse=float4(0.0f,0.0f,0.0f,0.0f);
   specular=float4(0.0f,0.0f,0.0f,0.0f);

   ambient=light.Ambient*mat.Ambient;

   float3 lightOppsite=-light.Direction;
   
   float diffuseFactor=dot(lightOppsite,normal);

   [flatten]// Flatten to avoid dynamic branching.
   if(diffuseFactor>0.0f)
   {
      diffuse=light.Diffuse*mat.Diffuse*diffuseFactor;

      float3 lightReflect=reflect(light.Direction,normal);//求反射光线方向
	  float  speFactor=pow(max(dot(lightReflect,toEye),0.0f),mat.Specular.w);//pow(b,n)返回b的n次方
	  specular=light.Specular*mat.Specular*speFactor;
   }
   
}

void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
				   out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = L.Position - pos;
		
	// The distance from surface to light.
	float d = length(lightVec);
	
	// Range test.
	if( d > L.Range )
		return;
		
	// Normalize the light vector.
	lightVec /= d; 
	
	// Ambient term.
	ambient = mat.Ambient * L.Ambient;	

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if( diffuseFactor > 0.0f )
	{
		float3 v         = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec    = specFactor * mat.Specular * L.Specular;
	}

	// Attenuate
	float att = 1.0f / dot(L.Att, float3(1.0f, d, d*d));

	ambient*=att;//让环境光也衰减
	diffuse *= att;
	spec    *= att;
}

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a spotlight.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
				  out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = L.Position - pos;
		
	// The distance from surface to light.
	float d = length(lightVec);
	
	// Range test.
	if( d > L.Range )
		return;
		
	// Normalize the light vector.
	lightVec /= d; 
	
	// Ambient term.
	ambient = mat.Ambient * L.Ambient;	

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if( diffuseFactor > 0.0f )
	{
		float3 v         = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec    = specFactor * mat.Specular * L.Specular;
	}
	
	// Scale by spotlight factor and attenuate.
	float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);

	// Scale by spotlight factor and attenuate.
	float att = spot / dot(L.Att, float3(1.0f, d, d*d));

	ambient *= spot;

	ambient*=att;//让环境光也衰减
	diffuse *= att;
	spec    *= att;
}
