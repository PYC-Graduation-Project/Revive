
struct InstanceData
{
    matrix world;
    matrix world_inverse_transpose;
};

struct MaterialData
{
    float4 base_color;
};

StructuredBuffer<InstanceData> g_instance_datas : register(t0, space0);
StructuredBuffer<MaterialData> g_material_datas : register(t1, space0);

cbuffer cbCameraData : register(b0, space0)
{
    matrix g_view;
    matrix g_projection;
    float3 g_camera_pos;
}

struct VS_OPAQUE_MESH_IN
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct VS_OPAQUE_MESH_OUT
{
    float4 sv_position : SV_POSITION;
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

VS_OPAQUE_MESH_OUT VSOpaqueMesh(VS_OPAQUE_MESH_IN input, uint instance_id : SV_InstanceID)
{
    VS_OPAQUE_MESH_OUT output;
    
    InstanceData i_data = g_instance_datas[instance_id];
    
    float4 position = mul(float4(input.position, 1.0f), i_data.world);
    output.position = position.xyz;
    output.sv_position = mul(mul(position, g_view), g_projection);
    output.normal = mul(input.normal, (float3x3)i_data.world);
    output.normal = normalize(output.normal);
    output.uv = input.uv;
    
    return output;
}

float4 PSOpaqueMesh(VS_OPAQUE_MESH_OUT input) : SV_TARGET
{
    //return g_material_datas[5].base_color;
    
    return float4(abs(input.normal), 1.0f);
}
