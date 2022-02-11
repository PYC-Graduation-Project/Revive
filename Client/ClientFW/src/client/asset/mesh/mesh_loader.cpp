#include "stdafx.h"
#include "client/asset/mesh/mesh_loader.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/bone/skeleton.h"
#include "client/asset/mesh/vertex.h"
#include "client/asset/mesh/index.h"
#include "client/asset/core/asset_manager.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/mesh/material.h"
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	SPtr<Mesh> MeshLoader::LoadMesh(const std::string& path, const std::string& extension)
	{
		SPtr<Mesh> mesh = nullptr;

		switch (HashCode(extension.c_str()))
		{
		case HashCode(".obj"):
			mesh = LoadObj(path, extension);
			break;
		default:
			LOG_ERROR("Files in {0} format cannot be supported", extension);
			break;
		}

		return mesh;
	}

	struct CombineData
	{
		std::string mtl_name;
		std::vector<UINT> pos_indices;
		std::vector<UINT> tex_indices;
		std::vector<UINT> normal_indices;
	};

	SPtr<StaticMesh> MeshLoader::LoadObj(const std::string& path, const std::string& extension)
	{
		std::ifstream obj_file(path);
		
		if (obj_file.is_open() == false)
		{
			LOG_ERROR("Could not find path : [{0}]", path);
			return nullptr;
		}

		std::string parent_path = file_help::GetParentPathFromPath(path);
		std::string stem = file_help::GetStemFromPath(path);

		UINT lod = 0;
		SPtr<StaticMesh> mesh = CreateSPtr<StaticMesh>();

		Vec3 max_pos{ -FLT_MAX, -FLT_MAX, -FLT_MAX };
		Vec3 min_pos{ FLT_MAX, FLT_MAX, FLT_MAX };

		auto UpdateMaxMin([&max_pos, &min_pos](const Vec3& pos) {
			max_pos.x = max(max_pos.x, pos.x);
			max_pos.y = max(max_pos.y, pos.y);
			max_pos.z = max(max_pos.z, pos.z);

			min_pos.x = min(min_pos.x, pos.x);
			min_pos.y = min(min_pos.y, pos.y);
			min_pos.z = min(min_pos.z, pos.z);
			});

		while (lod < 4)
		{
			std::vector<Vec3> positions;
			std::vector<Vec3> normals;
			std::vector<Vec2> tex_coords;
			std::map<std::string, SPtr<Material>> materials;
			std::vector<CombineData> combine_data;
			UINT combine_data_index = 0;

			std::stringstream ss;
			std::string line;
			std::string prefix;

			Vec3 temp_vec;
			UINT temp_uint = 0;
			std::string temp_string;

			while (std::getline(obj_file, line))
			{
				ss.clear();
				prefix.clear();
				ss.str(line);
				ss >> prefix;

				switch (HashCode(prefix.c_str()))
				{
				case HashCode("mtllib"):
				{
					ss >> temp_string;
					materials = AssetStore::LoadMaterials(file_help::GetParentPathFromPath(path) + "\\" + temp_string);
					break;
				}
				case HashCode("v"):
					ss >> temp_vec.x >> temp_vec.y >> temp_vec.z;

					temp_vec.z *= -1.0f;
					if(lod == 0)
						UpdateMaxMin(temp_vec);
					positions.emplace_back(std::move(temp_vec));
					break;
				case HashCode("vt"):
					ss >> temp_vec.x >> temp_vec.y >> temp_vec.z;
					temp_vec.y = 1.0f - temp_vec.y;
					tex_coords.emplace_back(Vec2{ temp_vec.x, temp_vec.y });
					break;
				case HashCode("vn"):
					ss >> temp_vec.x >> temp_vec.y >> temp_vec.z;
					temp_vec.z *= -1.0f;
					normals.emplace_back(std::move(temp_vec));
					break;
				case HashCode("usemtl"):
				{
					ss >> temp_string;
					auto iter = std::find_if(combine_data.cbegin(), combine_data.cend(),
						[temp_string](const CombineData& data) {
							return data.mtl_name == temp_string;
						});
					if (iter == combine_data.cend())
					{
						combine_data.push_back({ temp_string });
						combine_data_index = static_cast<UINT>(combine_data.size()) - 1;
					}
					else
					{
						combine_data_index = static_cast<UINT>(std::distance(combine_data.cbegin(), iter));
					}
				}
				break;
				case HashCode("f"):
				{
					int count = 0;

					while (ss >> temp_uint)
					{
						if (count == 0)
							combine_data[combine_data_index].pos_indices.emplace_back(temp_uint - 1);
						else if (count == 1)
							combine_data[combine_data_index].tex_indices.emplace_back(temp_uint - 1);
						else if (count == 2)
							combine_data[combine_data_index].normal_indices.emplace_back(temp_uint - 1);

						if (ss.peek() == '/')
						{
							++count;
							ss.ignore();
						}
						else if (ss.peek() == ' ')
						{
							ss.ignore(1, ' ');
							count = 0;
						}
					}
				}
				break;
				default:
					break;
				}
			}

			mesh->CreateDataForLodMesh(lod);

			UINT vertex_count = 0;
			for (const auto& data : combine_data)
				vertex_count += static_cast<UINT>(data.pos_indices.size());

			std::vector<TextureLightVertex> vertices(vertex_count);

			vertex_count = 0;
			for (const auto& data : combine_data)
			{
				UINT count = static_cast<UINT>(data.pos_indices.size());
				mesh->AddInstanceInfo(lod, { count, vertex_count });
				mesh->AddMaterial(lod, std::move(materials[data.mtl_name]));

				for (UINT i = 0; i < count; ++i)
				{
					UINT index = i + vertex_count;
					if (index % 3 == 0)
						index += 2;
					else if (index % 3 == 2)
						index -= 2;

					vertices[index].SetPosition(positions[data.pos_indices[i]]);
					vertices[index].SetTexCoord(tex_coords[data.tex_indices[i]]);
					vertices[index].SetNormal(normals[data.normal_indices[i]]);
				}

				vertex_count += count;
			}

			const UINT vertices_size = vertex_count * sizeof(TextureLightVertex);
			if (mesh->CreateVertexBufferBlob(lod, vertices_size) == false)
			{
				LOG_ERROR("Could not create blob for vertex");
				return nullptr;
			}
			CopyMemory(mesh->GetVertexBufferBlob(lod)->GetBufferPointer(), vertices.data(), vertices_size);

			++lod;
			std::string lod_path = parent_path + "/" + stem + "_lod" + std::to_string(lod) + extension;
			obj_file = std::ifstream(lod_path);

			if (obj_file.is_open() == false)
				break;
		}

		Vec3 center = max_pos + min_pos;
		Vec3 extents = (max_pos - min_pos) * 0.5f;
		extents.x = abs(extents.x), extents.y = abs(extents.y), extents.z = abs(extents.z);

		mesh->SetOrientBox(BOrientedBox{ center, extents });

		return mesh;
	}

	struct MeshData
	{
		std::vector<Vec3>positions;
		std::vector<Vec2>tex_coords;
		std::vector<Vec3>normals;
		std::vector<int> index_count; //안쓸예정 삭제예정
		std::vector<UINT> incdices;
		BOrientedBox oriented_box;
	};

	SPtr<Mesh> RevLoader::LoadMesh(const std::string& path, const std::string& extension)
	{
		SPtr<Mesh> mesh = nullptr;

		switch (HashCode(extension.c_str()))
		{
		case HashCode(".obj"):
			mesh = LoadObj(path, extension);
			break;
		case HashCode(".rev"):
			mesh = LoadRev(path, extension);
		default:
			LOG_ERROR("Files in {0} format cannot be supported", extension);
			break;
		}

		return mesh;
	}

	SPtr<SkeletalMesh> RevLoader::LoadRev(const std::string& path, const std::string& extension)
	{
		//std::ifstream rev_file(path);
		FILE* rev_file = NULL;
		fopen_s(&rev_file, path.c_str(), "rb");
		if (rev_file == NULL)
		{
			LOG_ERROR("Could not find path : [{0}]", path);
			return nullptr;
		}

		std::string parent_path = file_help::GetParentPathFromPath(path);
		std::string stem = file_help::GetStemFromPath(path);



		Vec3 max_pos{ -FLT_MAX, -FLT_MAX, -FLT_MAX };
		Vec3 min_pos{ FLT_MAX, FLT_MAX, FLT_MAX };

		auto UpdateMaxMin([&max_pos, &min_pos](const Vec3& pos) {
			max_pos.x = max(max_pos.x, pos.x);
			max_pos.y = max(max_pos.y, pos.y);
			max_pos.z = max(max_pos.z, pos.z);

			min_pos.x = min(min_pos.x, pos.x);
			min_pos.y = min(min_pos.y, pos.y);
			min_pos.z = min(min_pos.z, pos.z);
			});

		SPtr<SkeletalMesh> s_mesh = CreateSPtr<SkeletalMesh>();
		SPtr<Skeleton> skeleton = CreateSPtr<Skeleton>();

		std::stringstream ss;
		std::string line;
		std::string prefix;

		while (ReadStringFromFile(rev_file, &prefix))
		{

			switch (HashCode(prefix.c_str()))
			{
			case HashCode("<Hierarchy>"):
				LoadFrameHierArchy(rev_file, skeleton, s_mesh);
				s_mesh->SetSkeleton(skeleton);

				break;
			case HashCode("<Animation>"):

				break;
			case HashCode("</Animation>"):
				break;
			default:
				break;
			}
		}

		return s_mesh;
	}

	void RevLoader::LoadFrameHierArchy(FILE* rev_file, SPtr<Skeleton>& skeleton, SPtr<SkeletalMesh>& mesh)
	{
		std::stringstream ss;
		std::string line;
		std::string prefix;

		//skeleton
		std::string b_name;
		std::vector<Mat4> parent;
		Vec3 scale;
		Vec3 rotation;
		Vec3 translation;

		//mesh
		std::vector<MeshData> mesh_data;
		MeshData temp_data;
		UINT lod = 0;

		int n_frame = 0; //프레임 수
		int n_childs = 0;

		XMFLOAT4X4 temp_mat4;
		Vec3 temp_vec3;
		bool frame_end = false;
		bool h_end = false;

		while (ReadStringFromFile(rev_file, &prefix))
		{
			if (HashCode(prefix.c_str()) == HashCode("</Hierarchy>")) break;
			switch (HashCode(prefix.c_str()))
			{
			case HashCode("<Frame>:"):
				//rev_file.read((char*)&n_frame, sizeof(int));
				fread(&n_frame, sizeof(int), 1, rev_file);

				ReadStringFromFile(rev_file, &b_name); //bone name read
				skeleton->SetBoneName(b_name);
				while (ReadStringFromFile(rev_file, &prefix))
				{
					if (HashCode(prefix.c_str()) == HashCode("</Frame>")) break;
					switch (HashCode(prefix.c_str()))
					{
					case HashCode("<Transform>:"):

						//rev_file.read((char*)&temp_mat4, sizeof(XMFLOAT4X4));
						fread(&temp_mat4, sizeof(XMFLOAT4X4), 1, rev_file);
						//skeleton->SetToParent(temp_mat4);
						//fread(&temp_mat4, sizeof(XMFLOAT4X4), 1, pInFile);
						/*rev_file.read((char*)&skeleton->m_scale, sizeof(Vec3));
						rev_file.read((char*)&skeleton->m_rotation, sizeof(Vec3));
						rev_file.read((char*)&skeleton->m_translation, sizeof(Vec3));*/
						fread(&skeleton->m_scale, sizeof(Vec3), 1, rev_file);
						fread(&skeleton->m_rotation, sizeof(Vec3), 1, rev_file);
						fread(&skeleton->m_translation, sizeof(Vec3), 1, rev_file);
						break;

					case HashCode("<Mesh>:"):
						//LoadMeshFromFile(rev_file, mesh_data);
						break;

					case HashCode("<SkinDeformations>:"):
						LoadSkinDeformations(rev_file, mesh);
						ReadStringFromFile(rev_file, &prefix);
						if (HashCode(prefix.c_str()) == HashCode("<Mesh>:"))
						{
							LoadMeshFromFile(rev_file, temp_data);
							mesh_data.emplace_back(std::move(temp_data));
						}

						break;

					case HashCode("<Materals>:"):
						break;

					case HashCode("<Children>:"):
						//rev_file.read((char*)&n_childs, sizeof(int));
						fread(&n_childs, sizeof(int), 1, rev_file);
						if (n_childs > 0)
						{
							for (int i = 0; i < n_childs; ++i)
							{
								SPtr<Skeleton> child = CreateSPtr<Skeleton>();
								LoadFrameHierArchy(rev_file, child, mesh);
								if (child) skeleton->SetChild(child);
							}
						}

						break;

					case HashCode("</Frame>"):
						//frame_end = true;
						break;

					}
				}
				break;
			case HashCode("</Hierarchy>"):
				break;
				
			}
		}
		//mesh->SetOrientBox(mesh_data.oriented_box);

		std::vector<TextureLightVertex> vertices;
		std::vector<Index> indices;
		UINT vertex_count = 0;
		UINT index_count = 0;
		for (auto data : mesh_data) //메시가 여러개인데 합칠거임
		{
			
			UINT v_count = data.positions.size();
			for (UINT i = 0; i < v_count; ++i)
			{
				UINT index = i + vertex_count;
				vertices[index].SetPosition(data.positions[i]);
				vertices[index].SetTexCoord(data.tex_coords[i]);
				vertices[index].SetNormal(data.normals[i]);
			}

			vertex_count += v_count; //다음메시의 정점의 시작위치를 정한다.

			UINT i_count = data.incdices.size(); // 420 1000 600

			std::cout << "index_count[0] " << data.index_count[0];
			if (i_count == data.index_count[0])
			{
				std::cout << " == current_indices.size() "<< i_count << std::endl;
			}

			for (UINT i = 0; i < i_count; ++i) // 0~420 0~ 1000
			{
				UINT index = i + index_count; //0 ~ 420 420~1420
				indices[index].SetIndex(data.incdices[i]);
			}
			
			index_count += i_count; //다음메시의 정점의 인덱스위치를 정한다. 420 1420
			if (index_count > 0)
				mesh->AddInstanceInfo(lod, { i_count,index_count }); //420, 1420
			else
				mesh->AddInstanceInfo(lod, { v_count,vertex_count });
		}
		//Blob연결할차례
		//CopyMemory

	}
	
	void RevLoader::LoadMeshFromFile(FILE* rev_file, MeshData& mesh_data)
	{
		std::string mesh_name;
		ReadStringFromFile(rev_file, &mesh_name);

		std::vector<Vec3> positions;
		std::vector<Vec3> normals;
		std::vector<Vec2> tex_coords;
		//이 벡터의 크기 == sub_mesh_count
		std::vector<int> v_subset_index_count;
		std::vector<std::vector<UINT>> vv_subset_index_count; //여기도추가햇음

		//int sub_mesh_count = 0;

		std::string prefix;
		Vec3 temp_vec3;
		Vec2 temp_vec2;
		UINT temp_uint;
		std::vector<UINT> temp_v_uint;
		int temp_int;
		BoundingOrientedBox oriented_box;

		while (ReadStringFromFile(rev_file, &prefix))
		{
			switch (HashCode(prefix.c_str()))
			{
			case HashCode("<Bounds>:"):
				//rev_file.read((char*)&temp_vec3, sizeof(Vec3));
				fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
				mesh_data.oriented_box.SetCenter(temp_vec3);
				//rev_file.read((char*)&temp_vec3, sizeof(Vec3));
				fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
				mesh_data.oriented_box.SetExtents(temp_vec3);
				break;

			case HashCode("<ControlPoints>:"):
				//rev_file.read((char*)&temp_uint, sizeof(UINT));
				fread(&temp_uint, sizeof(UINT), 1, rev_file);

				//vertex_count 정점수를 받는 변수 저장은 하지 않음
				// 벡터 positions 크기와 같으니까
				if (temp_uint > 0)
				{

					//rev_file.read((char*)&temp_vec3, sizeof(Vec3));
					fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
					//temp_vec.z *= -1.0f; 필요할까?
					mesh_data.positions.emplace_back(std::move(temp_vec3));
				}
				break;

			case HashCode("<TextureCoords0>:"):
				//rev_file.read((char*)&temp_uint, sizeof(UINT));
				fread(&temp_uint, sizeof(UINT), 1, rev_file);
				// 
				//tex_coord_count
				if (temp_uint > 0)
				{
					//rev_file.read((char*)&temp_vec2, sizeof(Vec2));
					fread(&temp_vec2, sizeof(Vec2), 1, rev_file);

					mesh_data.tex_coords.emplace_back(std::move(temp_vec2));
				}

				break;

			case HashCode("<TextureCoords1>:"):
				break;

			case HashCode("<Normals>:"):
				//rev_file.read((char*)&temp_uint, sizeof(UINT));
				fread(&temp_uint, sizeof(UINT), 1, rev_file);
				//normal_count
				if (temp_uint > 0)
				{
					fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
					//rev_file.read((char*)&temp_vec3, sizeof(Vec3));
					mesh_data.normals.emplace_back(std::move(temp_vec3));
				}
				break;

			case HashCode("<Tangents>:"):
				fread(&temp_uint, sizeof(UINT), 1, rev_file);
				//rev_file.read((char*)&temp_uint, sizeof(UINT));
				//tan_count
				if (temp_uint > 0)
				{
					fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
					//rev_file.read((char*)&temp_vec3, sizeof(Vec3));
					//tangent.emplace_back
				}
				break;

			case HashCode("<BiTangents>:"):
				fread(&temp_uint, sizeof(UINT), 1, rev_file);
				//rev_file.read((char*)&temp_uint, sizeof(UINT));
				//bitan_count
				if (temp_uint > 0)
				{
					fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
					//rev_file.read((char*)&temp_vec3, sizeof(Vec3));
					//bitangent.emplace_back
				}
				break;

			case HashCode("<Polygons>:"):
				fread(&temp_int, sizeof(int), 1, rev_file);
				//rev_file.read((char*)&temp_int, sizeof(int));
				//Polgon_count
				while (ReadStringFromFile(rev_file, &prefix))
				{
					switch (HashCode(prefix.c_str()))
					{
					case HashCode("<SubIndices>:"):
						fread(&temp_int, sizeof(int), 1, rev_file);
						//rev_file.read((char*)&temp_int, sizeof(int));
						//index_count
						fread(&temp_int, sizeof(int), 1, rev_file);
						//rev_file.read((char*)&temp_int, sizeof(int));
						//sub_mesh_count = temp_int;
						//if (sub_mesh_count == 0) sub_mesh_count = 1;

						while (ReadStringFromFile(rev_file, &prefix))
						{
							switch (HashCode(prefix.c_str()))
							{
							case HashCode("<SubIndex>:"):
								fread(&temp_int, sizeof(int), 1, rev_file);
								//rev_file.read((char*)&temp_int, sizeof(int));
								//index_count
								fread(&temp_int, sizeof(int), 1, rev_file);
								//rev_file.read((char*)&temp_int, sizeof(int));
								mesh_data.index_count.emplace_back(std::move(temp_int));
								int subset_index_size = mesh_data.index_count.back();
								if (subset_index_size > 0)
								{
									for (int i = 0; i < subset_index_size; i++)
									{
										fread(&temp_uint, sizeof(UINT), 1, rev_file);
										//rev_file.read((char*)&temp_uint, sizeof(UINT));
										mesh_data.incdices.emplace_back(std::move(temp_uint));
									}
									//vv_subset_index_count.emplace_back(std::move(temp_v_uint));
								}
								break;
							}
						}

						break;
					case HashCode("</Polygons>:"):
						break;
					}
				}
				break;

			case HashCode("</Mesh>"):
				return;
			}
		}

	}

	void RevLoader::LoadSkinDeformations(FILE* rev_file, SPtr<SkeletalMesh>& mesh)
	{
		std::stringstream ss;
		std::string line;
		std::string prefix;

		Mat4 temp_mat4;
		Vec3 temp_vec3;
		Vec4 temp_vec4;
		IVec4 temp_ivec4;
		int temp_int;

		int skinning_bone_count = 0;
		int bone_per_vertex = 4;
		BOrientedBox oriented_box;

		while (ReadStringFromFile(rev_file, &prefix))
		{
			switch (HashCode(prefix.c_str()))
			{
			case HashCode("<BonesPerVertex>:"):
				fread(&bone_per_vertex, sizeof(int), 1, rev_file);
				//rev_file.read((char*)&bone_per_vertex, sizeof(int));
				break;
			case HashCode("<Bounds>:"):
				fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
				//rev_file.read((char*)&temp_vec3, sizeof(Vec3));
				oriented_box.SetCenter(temp_vec3);
				fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
				//rev_file.read((char*)&temp_vec3, sizeof(Vec3));
				oriented_box.SetExtents(temp_vec3);
				mesh->SetOrientBox(oriented_box);
				break;
			case HashCode("<BoneNames>:"):
				fread(&skinning_bone_count, sizeof(int), 1, rev_file);
				//rev_file.read((char*)&skinning_bone_count, sizeof(int));
				if (skinning_bone_count > 0)
				{
					for (int i = 0; i < skinning_bone_count; ++i)
					{
						ReadStringFromFile(rev_file, &prefix);
						mesh->AddBoneName(prefix);
					}
				}
				break;
			case HashCode("<BoneOffsets>:"):
				fread(&skinning_bone_count, sizeof(int), 1, rev_file);
				//rev_file.read((char*)&skinning_bone_count, sizeof(int));
				if (skinning_bone_count > 0)
				{

					for (int i = 0; i < skinning_bone_count; ++i)
					{
						fread(&temp_mat4, sizeof(Mat4), 1, rev_file);
						//rev_file.read((char*)&temp_mat4, sizeof(Mat4));
						temp_mat4.Transpose();
						mesh->AddBoneOffset(temp_mat4);
					}
				}
				break;
			case HashCode("<BoneIndices>:"):
				fread(&temp_int, sizeof(int), 1, rev_file);
				//rev_file.read((char*)&temp_int, sizeof(int)); //vetex_count
				if (temp_int > 0)
				{
					for (int i = 0; i < temp_int; ++i)
					{
						fread(&temp_ivec4, sizeof(IVec4), 1, rev_file);
						//rev_file.read((char*)&temp_ivec4, sizeof(IVec4));
						mesh->AddBoneIndex(temp_ivec4);
					}
				}
				break;
			case HashCode("<BoneWeights>:"):
				fread(&temp_int, sizeof(int), 1, rev_file);
				//rev_file.read((char*)&temp_int, sizeof(int)); //vetex_count
				if (temp_int > 0)
				{
					for (int i = 0; i < temp_int; ++i)
					{
						fread(&temp_vec4, sizeof(Vec4), 1, rev_file);
						//rev_file.read((char*)&temp_vec4, sizeof(Vec4));
						mesh->AddBoneWeight(temp_vec4);
					}
				}
				break;
			case HashCode("</SkinDeformations>"):
				break;
			}
		}
	}

	int RevLoader::ReadStringFromFile(FILE* file, std::string* word)
	{
		word->clear();
		int length = 0;
		char buffer[64] = { "\0" };
		//file.read((char*)&length, sizeof(int));
		fread(&length, sizeof(int), 1, file);
		//file.read(buffer, sizeof(char) * length);
		fread(&buffer, sizeof(char), length, file);
		*word = buffer;
		return length;
	}

	//int ReadFromFile(FILE* file, std::stringstream* word)
	//{
	//	word->clear();
	//	int length = 0;
	//	char buffer[64] = { "\0" };
	//	//file.read((char *)&length, sizeof(int));
	//	fread(&length, sizeof(int), 1, file);
	//	//file.read(buffer, sizeof(char) * length);
	//	fread(&buffer, sizeof(char), length, file);

	//	word->str(buffer);
	//	return length;
	//}
}
