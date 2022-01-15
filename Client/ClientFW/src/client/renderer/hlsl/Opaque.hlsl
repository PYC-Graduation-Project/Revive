
struct InstanceData
{
    matrix world;
    matrix world_inverse_transpose;
};

StructuredBuffer<InstanceData> g_instance_datas : register(t0, space0);

cbuffer cbCameraData : register(b0, space0)
{
    matrix g_view_proj;
    float3 g_camera_pos;
}

struct VS_OPAQUE_MESH_IN
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VS_OPAQUE_MESH_OUT
{
    float4 sv_position : SV_Position;
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

VS_OPAQUE_MESH_OUT VSOpaqueMesh(VS_OPAQUE_MESH_IN input, uint instance_id : SV_InstanceID)
{
    VS_OPAQUE_MESH_OUT output;
    
    InstanceData i_data = g_instance_datas[instance_id];
    
    float4 position = mul(float4(input.position, 1.0f), i_data.world);
    output.position = position.xyz;
    output.sv_position = position;
    output.normal = mul(input.normal, (float3x3) i_data.world_inverse_transpose);
    output.normal = normalize(output.normal);
    output.uv = input.uv;
    
    return output;
}

float4 PSOpaqueMesh(VS_OPAQUE_MESH_OUT input) : SV_TARGET
{
    return float4(input.normal, 1.0f);
}
