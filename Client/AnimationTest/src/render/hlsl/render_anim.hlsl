#include "../../../../ClientFW/src/client/renderer/hlsl/opaque.hlsl"

struct VS_SKINNED_MESH_IN
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float4 weights : BONEWEIGHT;
	int4 indices : BONEINDEX;
};

struct VS_SKINNED_MESH_OUT
{
	float4 sv_position : SV_POSITION;
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};


VS_SKINNED_MESH_OUT VSSkinnedMesh(VS_SKINNED_MESH_IN input, uint instance_id : SV_InstanceID)
{
	VS_SKINNED_MESH_OUT output;

	InstanceData i_data = g_instance_data[instance_id];
	BoneTransData b_data = g_transform_data[instance_id];
	
    float3 pos = float3(0.0f, 0.0f, 0.0f);
    float3 normal = float3(0.0f, 0.0f, 0.0f);
	

    pos += mul(float4(input.position, 1.0f), b_data.bone_trans[input.indices[0]]).xyz * input.weights[0];
    normal += mul(input.normal, (float3x3) b_data.bone_trans[input.indices[0]]) * input.weights[0];
	pos += mul(float4(input.position, 1.0f), b_data.bone_trans[input.indices[1]]).xyz * input.weights[1];
    normal += mul(input.normal, (float3x3) b_data.bone_trans[input.indices[1]]) * input.weights[1];
	pos += mul(float4(input.position, 1.0f), b_data.bone_trans[input.indices[2]]).xyz * input.weights[2];
    normal += mul(input.normal, (float3x3) b_data.bone_trans[input.indices[2]]) * input.weights[2];
	pos += mul(float4(input.position, 1.0f), b_data.bone_trans[input.indices[3]]).xyz * input.weights[3];
    normal += mul(input.normal, (float3x3) b_data.bone_trans[input.indices[3]]) * input.weights[3];
	
    
	//   float4x4 x1 = b_data.bone_trans[input.indices[0]] * input.weights[0];
	//   float4x4 x2 = b_data.bone_trans[input.indices[1]] * input.weights[1];
	//   float4x4 x3 = b_data.bone_trans[input.indices[2]] * input.weights[2];
	//   float4x4 x4 = b_data.bone_trans[input.indices[3]] * input.weights[3];
	//float4x4 vertex_to_bone_world = x1 + x2 + x3 + x4;
	
	//float4 position = mul(float4(input.position, 1.0f), i_data.world);
	//float4 position = mul(float4(input.position, 1.0f), vertex_to_bone_world);
	//float4 position = mul(mul(float4(input.position, 1.0f), vertex_to_bone_world), i_data.world);
	output.position = pos;
    output.sv_position = mul(mul(float4(pos, 1.0f), g_view), g_projection);
    output.normal = normal;
	//output.normal = normalize(output.normal);
	//output.normal = float3(input.indices.xxx);
	output.uv = input.uv;

	return output;
}

float4 PSSkinnedMesh(VS_SKINNED_MESH_OUT input) : SV_TARGET
{
 //return float4(input.normal, 1.0f);
    input.normal = normalize(input.normal);
	
	MaterialData material_data = g_material_data[g_material_index];
if (material_data.diffuse_texture_index >= 0)
{
	return g_texture_data[material_data.diffuse_texture_index].Sample(g_sampler_point_wrap, input.uv);

}
else
{
	return g_material_data[g_material_index].base_color;
}
}