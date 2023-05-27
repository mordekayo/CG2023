cbuffer CameraConstantBuffer : register(b0)
{
    row_major matrix ViewMatrix;
    row_major matrix ProjectionMatrix;
    row_major matrix ModelMatrix;
    float3 CameraPosition;
};

struct MaterialData
{
    float3 DiffuseReflectionCoefficient;
    float3 AbsorptionCoef;
    float3 AmbientConstant;
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
    float4 Position : POSITION0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
};

PS_IN VSMain(uint id : SV_VertexID)
{
    PS_IN Output = (PS_IN) 0;

    const float2 Indexes = float2(id & 1, (id & 2) >> 1);
    Output.pos = float4(Indexes * float2(2, -2) + float2(-1, 1), 0, 1);
    
    return Output;
}

Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D WorldPosMap : register(t2);
Texture2DArray ShadowMap : register(t3);

SamplerComparisonState ShadowMapSampler : register(s0);

struct GBufferData
{
    float4 DiffuseSpec;
    float3 Normal;
    float3 WorldPos;
};

GBufferData ReadGBuffer(float2 screenPos)
{
    GBufferData GBuffer = (GBufferData) 0;
    
    GBuffer.DiffuseSpec = DiffuseMap.Load(float3(screenPos, 0));
    GBuffer.WorldPos = WorldPosMap.Load(float3(screenPos, 0)).xyz;
    GBuffer.Normal = NormalMap.Load(float3(screenPos, 0)).xyz;
    
    return GBuffer;
}

float3 CalcDirLight(DirectionalLightData DirectionalLight, float3 Normal, float3 FromPointToCamera, GBufferData GBuffer, float4 PosViewProj, float Layer);

float4 PSMain(PS_IN input) : SV_Target
{
    GBufferData GBuffer = ReadGBuffer(input.pos.xy);
    
    float3 Normal = normalize(GBuffer.Normal);
    float3 ViewDirection = normalize(CameraPosition - GBuffer.WorldPos.xyz);
    
    float4 CameraViewPosition = mul(float4(GBuffer.WorldPos.xyz, 1.0f), ViewMatrix);
    
    float Layer = 3.0f;
    const float DepthVal = abs(CameraViewPosition.z);
    for (int i = 0; i < 4; ++i)
    {
        if (DepthVal < Distances[i])
        {
            Layer = (float) i;
            break;
        }
    }

    const float4 PosViewProj = mul(float4(GBuffer.WorldPos, 1.0f), ViewProj[Layer]);
    
    float3 result = CalcDirLight(DirectionalLight, Normal, ViewDirection, GBuffer, PosViewProj, Layer);

    return float4(result, 1.0f);
}

float IsLighted(float3 FromPointToCamera, float3 Normal, float4 PosViewProj, float Layer);

float3 CalcDirLight(DirectionalLightData DirectionalLight, float3 Normal, float3 FromPointToCamera, GBufferData GBuffer, float4 PosViewProj, float Layer)
{
    float3 DiffuseValue = GBuffer.DiffuseSpec;
    
    //DIRECTIONAL LIGHT
    const float3 NormalizedLightDirection = normalize(DirectionalLight.Direction);
    const float3 FromPointToLightSrc   = -NormalizedLightDirection;
    const float3 ReflectDirection = reflect(NormalizedLightDirection, Normal);

    const float3 Diffuse  = DiffuseValue * Material.DiffuseReflectionCoefficient * max(dot(FromPointToLightSrc, Normal), 0.0);
    const float3 Specular = DiffuseValue * Material.AbsorptionCoef * pow(max(dot(FromPointToCamera, ReflectDirection), 0.0), 128);
    const float3 Ambient  = DiffuseValue * Material.AmbientConstant * DirectionalLight.Color;
    
    float1 IsPixelLighted = 1;
    
    IsPixelLighted = IsLighted(FromPointToCamera, Normal, PosViewProj, Layer);
    
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
