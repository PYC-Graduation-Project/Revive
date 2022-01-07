
cbuffer cbDefaultPass : register(b0, space0)
{
    matrix gViewProj;
}

struct VS_DIFFUSE_IN
{
    float3 position : POSITION;
};

struct VS_DIFFUSE_OUT
{
    float4 position : SV_Position;
};

VS_DIFFUSE_OUT VSDiffuse(VS_DIFFUSE_IN input)
{
    VS_DIFFUSE_OUT output;
    output.position = float4(input.position, 1.0f);
    return output;
}

float4 PSDiffuse(VS_DIFFUSE_OUT input) : SV_TARGET
{
    return float4(1.0f, 1.0f, 0.0f, 0.0f);
}