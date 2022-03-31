#include "resource.hlsl"
#include "light.hlsl"
#include "ui.hlsl"

#ifndef __DEFERRED_HLSL__
#define __DEFERRED_HLSL__

//�ӽ�
//���� ���������� ���� �ʱ� ������ �׳� Texture���޷� ��

float ConvertToLinearDepth(float depth)
{
    return g_perspective_values.z / (depth + g_perspective_values.w);
}

float3 GetWorldPositionFromCSPos(float2 cs_pos, float depth)
{
    float3 position;
    
    position.xy = cs_pos.xy * g_perspective_values.xy * depth;
    position.z = depth;
    
    return mul(float4(position, 1.0f), g_inverse_view).xyz;
}


float3 GetWorldPositionFromUV(float2 uv, float depth)
{
    uv.x = uv.x * 2.0f - 1.0f;
    uv.y = (1.0f - uv.y) * 2.0f - 1.0f;
   
    return GetWorldPositionFromCSPos(uv, depth);
}


float4 PSRenderTexture(VS_RENDER_TEXTURE_OUTPUT input) : SV_TARGET
{
    return float4(g_texture_data[g_gbuffer_texture_indices[0]].Sample(g_sampler_point_wrap, input.uv).xyz, 1.0f);
}

float4 PSRenderTextureWithDirectionalLight(VS_RENDER_TEXTURE_OUTPUT input) : SV_TARGET
{
    float depth = g_texture_data[g_gbuffer_texture_indices[3]].Sample(g_sampler_point_wrap, input.uv).x;
    depth = ConvertToLinearDepth(depth);
    float3 position = GetWorldPositionFromUV(input.uv, depth);
    float3 normal = g_texture_data[g_gbuffer_texture_indices[1]].Sample(g_sampler_point_wrap, input.uv).xyz;
    normal = normalize(normal * 2.0f - 1.0f);
    float4 base_color = g_texture_data[g_gbuffer_texture_indices[0]].Sample(g_sampler_point_wrap, input.uv);
    float4 additional_info = g_texture_data[g_gbuffer_texture_indices[2]].Sample(g_sampler_point_wrap, input.uv);
    
    if(base_color.a > 0.0f)
    {
        Material material;
        material.base_color = base_color.xyz;
        material.normal = normal;
        material.roughness = additional_info.x;
        material.metalic = additional_info.y;
    
        float3 color = float3(0.f, 0.f, 0.f);
        [unroll(4)]
        for (uint i = 0; i < g_num_of_directional_light; ++i)
        {
            Light light;
            light.light_color = g_light_data[i].light_color;
            light.direction = g_light_data[i].light_direction;

            color += CalcDiretionalLight(position, material, light);
        }
    
     
        float3 ambient = 0.03f * material.base_color;
        color += ambient;
        color = color / (color + 1.0f);
        color = pow(color, (1.0f / 2.2f));
    
        return float4(color, 1.0f);
    }
    else
    {
        return float4(base_color.xyz, 1.0f);
    }  
}

struct VS_LOCAL_LIGHT_OUTPUT
{
    float4 sv_position : SV_POSITION;
    uint instance_id : SV_InstanceID;
};

struct HS_LOCAL_LIGHT_CONSTANT_DATA_OUTPUT
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct HS_LOCAL_LIGHT_OUTPUT
{
    float3 position : POSITION;
    uint instance_id : SV_InstanceID;
};

struct DS_LOCAL_LIGHT_OUTPUT
{
    float4 sv_position : SV_POSITION;
    float2 cs_position : CS_POSITION;
    uint light_index : LIGHT_INDEX;
};

VS_LOCAL_LIGHT_OUTPUT VSLocalLight(uint instance_id : SV_InstanceID)
{
    VS_LOCAL_LIGHT_OUTPUT output;
    
    output.sv_position = float4(0.0f, 0.0f, 0.0f, 1.0f);
    output.instance_id = instance_id;
    
    return output;
}

//
// Draw Point Light
//

HS_LOCAL_LIGHT_CONSTANT_DATA_OUTPUT ConstantHSPointLight()
{
    HS_LOCAL_LIGHT_CONSTANT_DATA_OUTPUT output;
    
    float tess_factor = 18.0f;
    output.edges[0] = output.edges[1] =
    output.edges[2] = output.edges[3] = tess_factor;
    output.inside[0] = output.inside[1] = tess_factor;
    
    return output;
}

static float3 hemil_dir[2] = { float3(1.0f, 1.0f, 1.0f), float3(-1.0f, 1.0f, -1.0f) };

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHSPointLight")]
HS_LOCAL_LIGHT_OUTPUT HSPointLight(InputPatch<VS_LOCAL_LIGHT_OUTPUT, 1> input, uint patch_id : SV_PrimitiveID)
{
    HS_LOCAL_LIGHT_OUTPUT output;
    
    output.position = hemil_dir[patch_id];
    output.instance_id = input[0].instance_id;
    
    return output;
}

[domain("quad")]
DS_LOCAL_LIGHT_OUTPUT DSPointLight(HS_LOCAL_LIGHT_CONSTANT_DATA_OUTPUT input, float2 uv : SV_DomainLocation, OutputPatch <HS_LOCAL_LIGHT_OUTPUT, 4> quad)
{
    float2 clip_space_uv = uv.xy * 2.0f - 1.0f;
    
    float2 clip_space_abs_uv = abs(clip_space_uv.xy);
    float max_length = max(clip_space_abs_uv.x, clip_space_abs_uv.y);
    
    float3 position = normalize(float3(clip_space_uv, (max_length - 1.0f)) * quad[0].position);
    
    DS_LOCAL_LIGHT_OUTPUT output;
    
    InstanceData instance_data = g_instance_data[quad[0].instance_id];
    
    output.sv_position = mul(mul(float4(position, 1.0f), instance_data.world), g_view_projection);
    output.cs_position = output.sv_position.xy / output.sv_position.w;
    output.light_index = instance_data.additional_info;
    
    return output;
}

float4 PSPointLight(DS_LOCAL_LIGHT_OUTPUT input) : SV_TARGET
{
    int3 location = int3(input.sv_position.xy, 0);
    float4 base_color = g_texture_data[g_gbuffer_texture_indices[0]].Load(location);
    
    if (base_color.a > 0.0f)
    {
        float depth = g_texture_data[g_gbuffer_texture_indices[3]].Load(location).x;
        depth = ConvertToLinearDepth(depth);
        float3 position = GetWorldPositionFromCSPos(input.cs_position, depth);
        float3 normal = g_texture_data[g_gbuffer_texture_indices[1]].Load(location).xyz;
        normal = normalize(normal * 2.0f - 1.0f);
        float4 additional_info = g_texture_data[g_gbuffer_texture_indices[2]].Load(location);
        
        Material material;
        material.base_color = base_color.xyz;
        material.normal = normal;
        material.roughness = additional_info.x;
        material.metalic = additional_info.y;
    
        float3 color = float3(0.f, 0.f, 0.f);
        
        LightData light_data = g_light_data[input.light_index];
        Light light;
            
        light.light_color = light_data.light_color;
        light.position = light_data.light_position;
        light.attenuation_radius = light_data.attenuation_radius;
        
        color += CalcPointLight(position, material, light);
        
        color = color / (color + 1.0f);
        color = pow(color, (1.0f / 2.2f));
        
        return float4(color, 1.0f);
    
    }
    else
    {
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}

#endif // __DEFERRED_HLSL__