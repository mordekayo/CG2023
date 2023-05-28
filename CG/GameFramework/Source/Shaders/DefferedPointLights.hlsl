cbuffer CameraConstantBuffer : register(b0)
{
    row_major matrix ViewMatrix;
    row_major matrix ProjectionMatrix;
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
    row_major matrix ModelMatrix;
    float3 Color;
    float4 ConstLinearQuadCount;
    float3 Position;
    float4 SphereRadius;
};
cbuffer LightConstantBuffer : register(b1)
{
    PointLightData PointLight;
};

struct VS_IN
{
    float4 Position : POSITION0;
};

struct PS_IN
{
    float4 Position      : SV_POSITION;
};

PS_IN VSMain(VS_IN Input)
{
    PS_IN Output = (PS_IN) 0; 
    Output.Position   = mul(mul(mul(float4(Input.Position.xyz, 1.0f), PointLight.ModelMatrix), ViewMatrix), ProjectionMatrix);  
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

float4 PSMain(PS_IN Input) : SV_Target
{
    GBufferData GBuffer = ReadGBuffer(Input.Position.xy);
    
    const float3 DiffuseValue = GBuffer.DiffuseSpec.rgb;
    const float3 Normal = GBuffer.Normal;
    const float3 FromPointToCamera = normalize(CameraPosition - GBuffer.WorldPos);
    
    const float3 FromPointToLightSrc = normalize(PointLight.Position - GBuffer.WorldPos);
    const float3 ReflectDirection = reflect(-FromPointToLightSrc, Normal);

    const float  Distance = length(PointLight.Position - GBuffer.WorldPos);
    if (Distance > PointLight.SphereRadius.x)
    {
        discard;
    }
    float Attenuation = max(1.0f - Distance / PointLight.SphereRadius.x, 0.0f);
    Attenuation *= Attenuation;
    
    const float3 Diffuse = DiffuseValue * max(dot(FromPointToLightSrc, Normal), 0.0) * PointLight.Color * Attenuation;
    const float3 Specular = DiffuseValue * pow(max(dot(FromPointToCamera, ReflectDirection), 0.0), 128) * PointLight.Color * Attenuation;
    
    return float4(float3(Diffuse + Specular), 0.0f);
}