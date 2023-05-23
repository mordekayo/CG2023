cbuffer CascBuf : register(b0)
{
    row_major matrix ModelMatrix;
    row_major matrix ViewProj[4];
};

struct VS_IN
{
    float3 Position : POSITION0;
};

struct GS_IN
{
    float4 Position : POSITION;
};

GS_IN VSMain(VS_IN Input)
{
    GS_IN Output = (GS_IN) 0;
    Output.Position = mul(float4(Input.Position.xyz, 1.0f), ModelMatrix);
    return Output;
}

struct GS_OUT
{
    float4 Position  : SV_POSITION;
    uint ArrayIndex : SV_RenderTargetArrayIndex;
};

[instance(4)]
[maxvertexcount(3)]
void GSMain(triangle GS_IN p[3], in uint id : SV_GSInstanceID, inout TriangleStream<GS_OUT> stream)
{
    [unroll]
    for (int i = 0; i < 3; ++i)
    {
        GS_OUT Gs = (GS_OUT) 0;
        Gs.Position = mul(float4(p[i].Position.xyz, 1.0f), ViewProj[id]);
        Gs.ArrayIndex = id;
        stream.Append(Gs);
    }
}