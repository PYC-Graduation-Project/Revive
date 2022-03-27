#ifndef __RESOURCE_HLSL__
#define __RESOURCE_HLSL__

static const float MASKED_ALPHA = 0.333333f;
static const float PI = 3.14159265359;

struct InstanceData
{
    matrix world;
    matrix world_inverse_transpose;
};

struct MaterialData
{
    float4 base_color;
    float roughness;
    float metallic;
    int diffuse_texture_index;
    int normal_texture_index;
    int roughness_texture_index;
    int metallic_texture_index;
};

struct LightData
{
    float3 light_color;
    float padding_0;
    float3 light_direction;
};

StructuredBuffer<InstanceData> g_instance_data : register(t0, space0);
StructuredBuffer<MaterialData> g_material_data : register(t1, space0);
StructuredBuffer<LightData> g_light_data : register(t2, space0);
Texture2D g_texture_data[] : register(t0, space1);
TextureCube g_texture_cube_data[] : register(t0, space2);

SamplerState g_sampler_point_wrap : register(s0);

cbuffer cbMaterialIndexData : register(b0, space0)
{
    uint g_material_index;
};

cbuffer cbCameraData : register(b1, space0)
{
    matrix g_view;
    matrix g_projection;
    matrix g_view_projection;
    matrix g_inverse_view;
    float4 g_perspective_values;
    float3 g_camera_pos;
    uint g_final_texture_index;
    uint4 g_gbuffer_texture_indices;
    uint g_num_of_directional_light;
}

#endif // __RESOURCE_HLSL__