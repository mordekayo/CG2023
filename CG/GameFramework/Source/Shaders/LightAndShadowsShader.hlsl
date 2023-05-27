cbuffer CameraConstantBuffer : register(b0)
{
    row_major matrix ViewMatrix;
    row_major matrix ProjectionMatrix;
    row_major matrix ModelMatrix;
    float3 CameraPosition;
};

struct MaterialData
{
    float3 AmbientConstant;
    float3 DiffuseReflectionCoefficient;
    float3 AbsorptionCoef;
};

struct DirectionalLightData
{
    float3 Color;
    float3 Direction;
};

struct PointLightData
{
    float3 Color;
    float4 ValueConLinQuadCount;
    float3 Position;
};

cbuffer LightConstantBuffer : register(b1)
{
	MaterialData Material;
    DirectionalLightData DirectionalLight;
    PointLightData PointLights[2];
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
    float4 CameraViewPosition : POSITION1;
    float4 Normal : NORMAL;
    float2 TexCoords : TEXCOORD;
};

PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN) 0;

    const float4 ModelPosition = mul(float4(input.Position, 1.0f), ModelMatrix);
    output.Position = mul(mul(ModelPosition, ViewMatrix), ProjectionMatrix);
    output.ModelPosition = ModelPosition;
    output.CameraViewPosition = mul(ModelPosition, ViewMatrix);
    output.Normal = mul(transpose(ModelMatrix), input.Normal);
    output.TexCoords = input.TexCoords;
    
    return output;
}

Texture2D DiffuseMap                    : register(t0);
SamplerState Sampler                    : register(s0);

Texture2DArray ShadowMap                : register(t1);
SamplerComparisonState ShadowMapSampler : register(s1);

float3 CalcDirLight(float4 ModelPos, MaterialData Material, float3 Normal, float3 FromPointToCamera, float2 TexCoords, float4 DirLightViewProj, float Layer);

float4 PSMain(PS_IN Input) : SV_Target
{
    const float3 Normal    = normalize(Input.Normal);
    const float3 FromPointToCamera = normalize(CameraPosition - Input.ModelPosition.xyz);
    
    float Layer = 3.0f;
    const float Depth = abs(Input.CameraViewPosition.z);
    for (int i = 0; i < 4; ++i)
    {
        if (Depth < Distances[i])
        {
            Layer = (float) i;
            break;
        }
    }

    const float4 PosViewProj = mul(Input.ModelPosition, ViewProj[Layer]);
    
    float3 result = CalcDirLight(Input.ModelPosition, Material, Normal, FromPointToCamera, Input.TexCoords, PosViewProj, Layer);

    return float4(result, 1.0f);
}

float IsLighted(float3 FromPointToCamera, float3 Normal, float4 PosViewProj, float Layer);

float3 CalcDirLight(float4 modelPos, MaterialData Material, float3 Normal, float3 FromPointToCamera, float2 TexCoords, float4 dirLightViewProj, float layer)
{
    const float3 DiffuseValue = DiffuseMap.Sample(Sampler, TexCoords).rgb;
    
    // DIRECTIONAL LIGHT
    const float3 NormalizedLightDirection = normalize(DirectionalLight.Direction);
    const float3 FromPointToLightSrc   = -NormalizedLightDirection;
    const float3 ReflectDirection = reflect(NormalizedLightDirection, Normal);
    
    float3 Diffuse  = DiffuseValue * Material.DiffuseReflectionCoefficient * max(dot(FromPointToLightSrc, Normal), 0.0);
    float3 Specular = DiffuseValue * Material.AbsorptionCoef * pow(max(dot(FromPointToCamera, ReflectDirection), 0.0), 128);
    float3 Ambient  = DiffuseValue * Material.AmbientConstant * DirectionalLight.Color;
    
    // POINT LIGHTS
    for (int i = 0; i < PointLights[i].ValueConLinQuadCount.w; i++)
    {
        const float Distance = length(PointLights[i].Position - modelPos.xyz);
        const float Attenuation = 1.0f / (PointLights[i].ValueConLinQuadCount.x + PointLights[i].ValueConLinQuadCount.y * Distance + PointLights[i].ValueConLinQuadCount.z * (Distance * Distance));
        Ambient  += Ambient  * Attenuation * PointLights[i].Color;
        Diffuse  += Diffuse  * Attenuation * PointLights[i].Color;
        Specular += Specular * Attenuation * PointLights[i].Color;
    }
    
    float1 IsPixelLighted = 1;
    IsPixelLighted = IsLighted(NormalizedLightDirection, Normal, dirLightViewProj, layer);
    
    return (Ambient + (Diffuse + Specular) * IsPixelLighted);
}

float IsLighted(float3 FromPointToCamera, float3 Normal, float4 PosViewProj, float Layer)
{
    const float NormalDotCamera = dot(Normal, FromPointToCamera);
    const float Bias = clamp(0.005f * (1.0f - NormalDotCamera), 0.0f, 0.0005f);
    
    float3 ProjectTexCoord;

    ProjectTexCoord.x = PosViewProj.x / PosViewProj.w;
    ProjectTexCoord.y = PosViewProj.y / PosViewProj.w;
    ProjectTexCoord.z = PosViewProj.z / PosViewProj.w;

    ProjectTexCoord.x = ProjectTexCoord.x * 0.5 + 0.5f;
    ProjectTexCoord.y = ProjectTexCoord.y * -0.5 + 0.5f;

    const float MaxDepth = ShadowMap.SampleCmpLevelZero(ShadowMapSampler, float3(ProjectTexCoord.x, ProjectTexCoord.y, Layer), ProjectTexCoord.z - Bias);
    
    return MaxDepth;
}

