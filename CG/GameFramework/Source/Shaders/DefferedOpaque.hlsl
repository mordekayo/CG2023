cbuffer CameraConstantBuffer : register(b0)
{
    row_major matrix ViewMatrix;
    row_major matrix ProjectionMatrix;
    row_major matrix ModelMatrix;
    float3 CameraPosition;
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
    float4 Normal : NORMAL;
    float2 TexCoords : TEXCOORD;
};

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

PS_IN VSMain(VS_IN Input)
{
    PS_IN Output = (PS_IN) 0;

    const float4 ModelPosition = mul(float4(Input.Position, 1.0f), ModelMatrix);
    Output.Position = mul(mul(ModelPosition, ViewMatrix), ProjectionMatrix);
    Output.ModelPosition = ModelPosition;
    Output.Normal = mul(transpose(ModelMatrix), Input.Normal);
    Output.TexCoords = Input.TexCoords;
    
    return Output;
}

struct PSOutput
{
    float4 Diffuse : SV_Target0;
    float4 Normal : SV_Target1;
    float4 WorldPos : SV_Target2;
};

[earlydepthstencil]
PSOutput PSMain(PS_IN Input)
{
    PSOutput Output = (PSOutput) 0;
    
    Output.Diffuse.rgb = DiffuseMap.Sample(Sampler, Input.TexCoords).rgb;
    Output.WorldPos = Input.ModelPosition;
    Output.Diffuse.a = float3(1, 1, 1);

    const float3 Normal = Input.Normal;
    float3 UnpackedNormal = normalize(Normal * 2.0f - 1.0f);
    Output.Normal = float4(UnpackedNormal, 0);
    
    return Output;
}