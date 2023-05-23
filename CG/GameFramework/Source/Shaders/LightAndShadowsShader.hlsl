cbuffer CameraConstantBuffer : register(b0)
{
    row_major matrix ViewMatrix;
    row_major matrix ProjectionMatrix;
    row_major matrix ModelMatrix;
    float3 CameraPosition;
};

struct LightData
{
    float3 Direction;
    float3 AmbientConstant;
    float3 DiffuseReflectionCoefficient;
    float3 AbsorptionCoef;
};
cbuffer LightConstantBuffer : register(b1)
{
	LightData Light;
};

cbuffer LightCameraConstantBuffer : register(b2)
{   
    row_major matrix ViewProj[4];
    float4 Distances;
};

struct VS_IN
{
    float3 Position : POSITION0;
    float2 TexCoords : TEXCOORD0;
    float4 Normal : NORMAL0;
};

struct PS_IN
{
    float4 Position : SV_POSITION;
    float4 ModelPosition : POSITION;
    float4 LightViewPosition : POSITION1;
    float4 Normal : NORMAL;
    float2 TexCoords : TEXCOORD;
};

PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN) 0;

    const float4 ModelPosition = mul(float4(input.Position, 1.0f), ModelMatrix);
    output.Position = mul(mul(ModelPosition, ViewMatrix), ProjectionMatrix);
    output.ModelPosition = ModelPosition;
    output.LightViewPosition = mul(ModelPosition, ViewMatrix);
    output.Normal = mul(transpose(ModelMatrix), input.Normal);
    output.TexCoords = input.TexCoords;
    
    return output;
}

Texture2D DiffuseMap                    : register(t0);
SamplerState Sampler                    : register(s0);

Texture2DArray ShadowMap                : register(t1);
SamplerComparisonState ShadowMapSampler : register(s1);

float3 CalcDirLight(LightData Light, float3 Normal, float3 FromPointToCamera, float2 TexCoords, float4 PosViewProj, float Layer);

float4 PSMain(PS_IN input) : SV_Target
{
    const float3 Normal    = normalize(input.Normal);
    const float3 FromPointToCamera = normalize(CameraPosition - input.ModelPosition.xyz);
    
    float Layer = 3.0f;
    const float Depth = abs(input.LightViewPosition.z);
    for (int i = 0; i < 4; ++i)
    {
        if (Depth < Distances[i])
        {
            Layer = (float) i;
            break;
        }
    }

    const float4 PosViewProj = mul(input.ModelPosition, ViewProj[Layer]);
    
    float3 result = CalcDirLight(Light, Normal, FromPointToCamera, input.TexCoords, PosViewProj, Layer);

    return float4(result, 1.0f);
}

float IsLighted(float3 FromPointToCamera, float3 Normal, float4 PosViewProj, float Layer);

float3 CalcDirLight(LightData Light, float3 Normal, float3 FromPointToCamera, float2 TexCoords, float4 PosViewProj, float Layer)
{
    const float3 DiffuseValue = DiffuseMap.Sample(Sampler, TexCoords).rgb;
    const float3 FromPointToLightSrc = normalize(-Light.Direction);

    const float3 ReflectDirection = reflect(Light.Direction, Normal);

    const float3 Diffuse  = DiffuseValue * Light.DiffuseReflectionCoefficient * max(dot(FromPointToLightSrc, Normal), 0.0);
    const float3 Specular = DiffuseValue * Light.AbsorptionCoef * pow(max(dot(FromPointToCamera, ReflectDirection), 0.0), 128);
    const float3 Ambient  = DiffuseValue * Light.AmbientConstant;

    float1 isLighted = 1;   
    isLighted = IsLighted(FromPointToCamera, Normal, PosViewProj, Layer);
    
    return (float3(0.3f, 0.3f, Layer * 0.3f) + Ambient + (Diffuse + Specular) * isLighted);
}

float IsLighted(float3 FromPointToCamera, float3 Normal, float4 PosViewProj, float Layer)
{
    float NormalDotCamera = dot(Normal, FromPointToCamera);
    float Bias = clamp(0.005f * (1.0f - NormalDotCamera), 0.0f, 0.0005f);
    
    float3 ProjectTexCoord;

    ProjectTexCoord.x = PosViewProj.x / PosViewProj.w;
    ProjectTexCoord.y = PosViewProj.y / PosViewProj.w;
    ProjectTexCoord.z = PosViewProj.z / PosViewProj.w;

    ProjectTexCoord.x = ProjectTexCoord.x * 0.5 + 0.5f;
    ProjectTexCoord.y = ProjectTexCoord.y * -0.5 + 0.5f;
    
    float MaxDepth = ShadowMap.SampleCmpLevelZero(ShadowMapSampler, float3(ProjectTexCoord.x, ProjectTexCoord.y, Layer), ProjectTexCoord.z - Bias);
    
    return MaxDepth;
}