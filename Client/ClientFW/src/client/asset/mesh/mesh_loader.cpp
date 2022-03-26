#include "stdafx.h"
#include "client/asset/mesh/mesh_loader.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/primitive/vertex.h"
#include "client/asset/bone/skeleton.h"
#include "client/asset/texture/texture.h"
#include "client/asset/mesh/index.h"
#include "client/asset/animation/animation_sequence.h"
#include "client/asset/core/asset_manager.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/material/material.h"
#include "client/physics/core/bounding_mesh.h"
#include "client/physics/collision/mesh_bounding_tree.h"

namespace client_fw
{
	SPtr<Mesh> MeshLoader::LoadMesh(const std::string& path, const std::string& extension) const
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

	SPtr<StaticMesh> MeshLoader::LoadObj(const std::string& path, const std::string& extension) const
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

			std::vector<TextureLightNormalMapVertex> vertices;
			vertices.reserve(vertex_count);
			std::vector<Triangle> triangles;
			triangles.reserve(vertex_count / 3);

			vertex_count = 0;
			for (const auto& data : combine_data)
			{
				UINT count = 0;
				for (size_t i = 0; i < data.pos_indices.size() / 3; ++i)
				{
					size_t index = i * 3;

					Vec3 v1 = positions[data.pos_indices[index + 2]];
					Vec3 v2 = positions[data.pos_indices[index + 1]];
					Vec3 v3 = positions[data.pos_indices[index]];

					Vec3 normal = vec3::Cross(v3 - v1, v2 - v1, true);
					if (normal == vec3::ZERO) continue;

					triangles.emplace_back(Triangle{ v1, v2, v3, normal });

					Vec2 uv1 = tex_coords[data.tex_indices[index + 2]];
					Vec2 uv2 = tex_coords[data.tex_indices[index + 1]];
					Vec2 uv3 = tex_coords[data.tex_indices[index]];

					Vec3 delta_pos1 = v2 - v1;
					Vec3 delta_pos2 = v3 - v1;

					Vec2 delta_uv1 = uv2 - uv1;
					Vec2 delta_uv2 = uv3 - uv1;

					float f = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv2.x + delta_uv1.y);

					Vec3 tangent;
					tangent.x = f * (delta_uv2.y * delta_pos1.x - delta_uv1.y * delta_pos2.x);
					tangent.y = f * (delta_uv2.y * delta_pos1.y - delta_uv1.y * delta_pos2.y);
					tangent.z = f * (delta_uv2.y * delta_pos1.z - delta_uv1.y * delta_pos2.z);

					Vec3 bitangent;
					bitangent.x = f * (-delta_uv2.x * delta_pos1.x + delta_uv1.x * delta_pos2.x);
					bitangent.y = f * (-delta_uv2.x * delta_pos1.y + delta_uv1.x * delta_pos2.y);
					bitangent.z = f * (-delta_uv2.x * delta_pos1.z + delta_uv1.x * delta_pos2.z);

					for (INT j = 2; j >= 0; --j)
					{
						TextureLightNormalMapVertex vertex;
						vertex.SetPosition(positions[data.pos_indices[index + j]]);
						vertex.SetTexCoord(tex_coords[data.tex_indices[index + j]]);
						vertex.SetNormal(normals[data.normal_indices[index + j]]);
						vertex.SetTangent(tangent);
						vertex.SetBitangent(bitangent);
						vertices.emplace_back(std::move(vertex));
					}			

					count += 3;
				}

				mesh->AddMeshVertexInfo(lod, { count, vertex_count });
				mesh->AddMaterial(lod, std::move(materials[data.mtl_name]));

				vertex_count += count;
			}

			const auto& vertex_info = mesh->GetVertexInfo(lod);
			if(vertex_info->CreateVertexBlob<TextureLightNormalMapVertex>(vertex_count)==false)
			{
				LOG_ERROR("Could not create blob for vertex");
				return nullptr;
			}
			vertex_info->CopyData(vertices.data(), vertex_count);

			if (lod == 0)
			{
#ifdef SHOW_TREE_INFO
				LOG_INFO("Triangle Tree : {0}", path);
#endif // SHOW_TREE_INFO
				BOrientedBox box = BOrientedBox(std::move(positions));
				mesh->SetOrientBox(box);
				/*auto bounding_tree = CreateSPtr<KDTree>();
				bounding_tree->Initialize(box, triangles);
				mesh->SetBoundingTree(std::move(bounding_tree));*/
			}

			++lod;
			std::string lod_path = parent_path + "/" + stem + "_lod" + std::to_string(lod) + extension;
			obj_file = std::ifstream(lod_path);

			if (obj_file.is_open() == false)
				break;
		}

		return mesh;
	}

	struct MeshData
	{
		std::vector<Vec3>positions;
		std::vector<Vec2>tex_coords;
		std::vector<Vec3>normals;
		std::vector<UINT> incdices;
		std::vector<std::string> mtl_names;
		std::map<std::string, SPtr<Material>> materials;
		SPtr<BoneData> bone_data;
		std::vector<BOrientedBox> oriented_boxes;

	};

	SPtr<Mesh> RevLoader::LoadMesh(const std::string& path, const std::string& extension) const
	{
		SPtr<Mesh> mesh = nullptr;
		static_cast<UINT>(m_mesh_count) = 0;
		switch (HashCode(extension.c_str()))
		{
		case HashCode(".obj"):
			mesh = LoadObj(path, extension);
			break;
		case HashCode(".rev"):
			mesh = LoadRev(path, extension);
			break;
		default:
			LOG_ERROR("Files in {0} format cannot be supported", extension);
			break;
		}

		return mesh;
	}

	SPtr<SkeletalMesh> RevLoader::LoadRev(const std::string& path, const std::string& extension) const
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

		std::string prefix;

		UINT lod = 0;

		SPtr<SkeletalMesh> s_mesh = CreateSPtr<SkeletalMesh>();
		SPtr<Skeleton> skeleton = CreateSPtr<Skeleton>();
		skeleton->SetBoneName("Root");
		
		//정보를 모으는 벡터들은 재귀함수 바깥쪽에 있어야 한다
		std::vector<MeshData> mesh_data;
		InitializeMeshData(mesh_data);

		while (ReadStringFromFile(rev_file, &prefix))
		{
			if (prefix.compare("</Animation>") == false) break;

			switch (HashCode(prefix.c_str()))
			{
			case HashCode("<Hierarchy>"):

				while (ReadStringFromFile(rev_file, &prefix))
				{
					if (prefix.compare("<Frame>:") == false)
					{
						SPtr<Skeleton> child = CreateSPtr<Skeleton>();
						skeleton->SetChild(child);
						LoadFrameHierArchy(rev_file, child, mesh_data, path);
					}
					else if ("</Hierarchy>")
					{
						SaveRevData(s_mesh, lod, mesh_data);
						s_mesh->SetSkeleton(skeleton);
		
						break;
					}
				}
				
				//return s_mesh;
				break;
			case HashCode("<Animation>"):
				AssetStore::LoadAnimation(rev_file, s_mesh->GetSkeleton(), path);
				break;
			default:
				break;
			}
		}

		return s_mesh;
	}

	FILE* RevLoader::LoadRevForAnimation(const std::string& path, const std::string& extension) const
	{
		FILE* rev_file = NULL;
		fopen_s(&rev_file, path.c_str(), "rb");
		if (rev_file == NULL)
		{
			LOG_ERROR("Could not find path : [{0}]", path);
			return nullptr;
		}

		std::string parent_path = file_help::GetParentPathFromPath(path);
		std::string stem = file_help::GetStemFromPath(path);

		std::string prefix;

		SPtr<SkeletalMesh> s_mesh = CreateSPtr<SkeletalMesh>();
		SPtr<Skeleton> skeleton = CreateSPtr<Skeleton>();

		//정보를 모으는 벡터들은 재귀함수 바깥쪽에 있어야 한다
		std::vector<MeshData> mesh_data;
		InitializeMeshData(mesh_data);

		while (ReadStringFromFile(rev_file, &prefix))
		{
			if (prefix.compare("</Animation>") == false) break;

			switch (HashCode(prefix.c_str()))
			{
			case HashCode("<Hierarchy>"):

				while (ReadStringFromFile(rev_file, &prefix))
				{
					if (prefix.compare("<Frame>:") == false)
					{
						SPtr<Skeleton> child = CreateSPtr<Skeleton>();
						LoadFrameHierArchy(rev_file, child, mesh_data, path);
					}
					else if ("</Hierarchy>")
						break;
				}
				break;
			case HashCode("<Animation>"):
				return rev_file;
				//AssetStore::LoadAnimation(rev_file, s_mesh->GetSkeleton(), path);
				break;
			default:
				break;
			}
		}

		return nullptr;
	}

	void RevLoader::SaveRevData(SPtr<SkeletalMesh>& s_mesh, const UINT& lod, std::vector<MeshData>& mesh_data) const
	{
		s_mesh->CreateDataForLodMesh(lod);
		s_mesh->SetBoneData(mesh_data.at(0).bone_data);
		UINT mesh_index = (m_mesh_count == 0) ? 0 : m_mesh_count - 1;

		UINT vertex_count = 0;
		UINT index_count = 0;

		for (auto& data : mesh_data)
		{
			for (auto& name : data.mtl_names)
			{
				auto temp_material = data.materials[name];
				if (temp_material)s_mesh->AddMaterial(lod, std::move(temp_material));
			}

			vertex_count += static_cast<UINT>(data.positions.size());
			index_count += static_cast<UINT>(data.incdices.size());

			//현재 obb데이터가 비어있음
			/*for (auto& oriented_box : data.oriented_boxes)
			{
			mesh->SetOrientBox(std::move(oriented_box));
			}*/
		}

		std::vector<BoneVertex> vertices(vertex_count);
		std::vector<UINT> indices(index_count);

		vertex_count = 0;
		index_count = 0;

		for (auto& data : mesh_data) //메시가 여러개인데 합칠거임
		{

			UINT v_count = static_cast<UINT>(data.positions.size());
			for (UINT i = 0; i < v_count; ++i)
			{
				UINT index = i + vertex_count;
				vertices[index].SetPosition(data.positions[i]);
				vertices[index].SetTexCoord(data.tex_coords[i]);
				vertices[index].SetNormal(data.normals[i]);
				vertices[index].SetBoneWeight(data.bone_data->bone_weights[i]);
				vertices[index].SetBoneIndex(data.bone_data->bone_indices[i]);
			}


			UINT i_count = static_cast<UINT>(data.incdices.size());

			/*std::cout << "index_count[0] " << data.index_count[0];
			if (i_count == data.index_count[0])
			{
			std::cout << " == current_indices.size() "<< i_count << std::endl;
			}*/

			for (UINT i = 0; i < i_count; ++i)
			{
				UINT index = i + index_count;
				indices[index] = data.incdices[i];
			}

			//if(false)
			if (i_count > 0)
			{
				s_mesh->AddMeshVertexInfo(lod, { i_count,index_count });
				s_mesh->SetDrawIndex(true);
			}
			else
				s_mesh->AddMeshVertexInfo(lod, { v_count,vertex_count });

			vertex_count += v_count; //다음메시의 정점의 시작위치를 정한다.
			index_count += i_count; //다음메시의 정점의 인덱스위치를 정한다.



		}
		//Blob연결

		const auto& vertex_info = s_mesh->GetVertexInfo(lod);
		if (vertex_info->CreateVertexBlob<BoneVertex>(vertex_count) == false)
		{
			LOG_ERROR("Could not create blob for vertex");
			return ;
		}
		vertex_info->CopyData(vertices.data(), vertex_count);

		const auto& index_info = s_mesh->GetIndexInfo(lod);
		if (index_info->CreateIndexBlob(index_count) == false)
		{
			LOG_ERROR("Could not create blob for index");
			return ;
		}
		index_info->CopyData(indices.data(), index_count);
	}


	bool RevLoader::LoadFrameHierArchy(FILE* rev_file, SPtr<Skeleton>& skeleton, std::vector<MeshData>& mesh_data, const std::string& path) const
	{
		std::string prefix;


		std::string parent_path = file_help::GetParentPathFromPath(path);

		//skeleton
		std::string b_name;

		//mesh
		UINT lod = 0;

		int n_frame = 0; //프레임 수
		int n_childs = 0;
		int material_count = 0;
		std::string texture_name;

		int temp_int = 0;
		Mat4 temp_mat4;
		Vec3 temp_vec3;

		fread(&n_frame, sizeof(int), 1, rev_file);
		ReadStringFromFile(rev_file, &b_name); //bone name read
		skeleton->SetBoneName(b_name);
		UINT mesh_index = 0;
		if(this != nullptr)
			 mesh_index = (m_mesh_count == 0) ? 0 : m_mesh_count - 1;
		auto& temp_data = mesh_data.at(mesh_index);
		//mesh_data.at(0).bone_data->bone_names.emplace_back(std::move(b_name));

		while (ReadStringFromFile(rev_file, &prefix))
		{
			//auto hash_code = HashCode(prefix.c_str());

			if (prefix.compare("<Transform>:") == 0)
			{
				//rev_file.read((char*)&temp_mat4, sizeof(Mat4));
				fread(&temp_mat4, sizeof(Mat4), 1, rev_file);
				skeleton->SetToParent(temp_mat4);
				//fread(&temp_mat4, sizeof(Mat4), 1, pInFile);
				/*rev_file.read((char*)&skeleton->m_scale, sizeof(Vec3));
				rev_file.read((char*)&skeleton->m_rotation, sizeof(Vec3));
				rev_file.read((char*)&skeleton->m_translation, sizeof(Vec3));*/
				fread(&skeleton->m_scale, sizeof(Vec3), 1, rev_file);
				fread(&skeleton->m_rotation, sizeof(Vec3), 1, rev_file);
				fread(&skeleton->m_translation, sizeof(Vec3), 1, rev_file);
			}

			else if (prefix.compare("<Mesh>:") == 0) //일반 메쉬 (미처리)
			{
				LoadMeshFromRevFile(rev_file, mesh_data);
				if (this != nullptr)AddMesh();
			}
			else if (prefix.compare("<SkinDeformations>:") == 0)
			{
				if (this != nullptr)
				{
					AddMesh();
					if (m_mesh_count > 1) InitializeMeshData(mesh_data);
				}
				LoadSkinDeformations(rev_file, mesh_data.at(0).bone_data); //뼈정보는 한곳에 몰아넣기
				ReadStringFromFile(rev_file, &prefix);
				if (!prefix.compare("<Mesh>:"))
				{
					LoadMeshFromRevFile(rev_file, mesh_data);
				}

			}
			else if (prefix.compare("<Materials>:") == 0)
			{

				fread(&material_count, sizeof(int), 1, rev_file);
				while (ReadStringFromFile(rev_file, &prefix))
				{
					if (prefix.compare("</Materials>") == 0) break;
					switch (HashCode(prefix.c_str()))
					{
					case HashCode("<Material>:"):
						fread(&temp_int, sizeof(int), 1, rev_file);
						break;
					case HashCode("<AlbedoMap>:"):
						ReadStringFromFile(rev_file, &texture_name); //W_HEAD_00_violet +확장자 붙힌채로 읽기
						temp_data.mtl_names.push_back(texture_name );
						temp_data.materials = AssetStore::LoadMaterials(parent_path + "/" + texture_name + ".mtl");
						//AddRevMaterial(temp_data.materials,texture_name, file_help::GetParentPathFromPath(path));
						break;
					}
				}
			}

			else if (prefix.compare("<Children>:") == 0)
			{
				//rev_file.read((char*)&n_childs, sizeof(int));
				fread(&n_childs, sizeof(int), 1, rev_file);
				if (n_childs > 0)
				{
					for (int i = 0; i < n_childs; ++i)
					{
						ReadStringFromFile(rev_file, &prefix);
						if (prefix.compare("<Frame>:") == 0)
						{
							SPtr<Skeleton> child = CreateSPtr<Skeleton>();
							skeleton->SetChild(child);

							LoadFrameHierArchy(rev_file, child, mesh_data, path);

						}

					}
				}
			}
			else if (prefix.compare("</Frame>") == 0)
			{
				if (skeleton->GetParent())return false; //한 프레임이 끝낫는데 부모가 있으면 루트가아니므로 재귀 탈출
				else break;//루트 프레임이면 반복문을 탈출하고 읽은 정보를 저장한다.
			}
		}

		return true;
	}

	void RevLoader::LoadMeshFromRevFile(FILE* rev_file, std::vector<MeshData>& mesh_data) const
	{
		std::string mesh_name;
		ReadStringFromFile(rev_file, &mesh_name);

		MeshData temp_mdata;

		std::string prefix;
		Vec3 temp_vec3;
		Vec2 temp_vec2;
		UINT temp_uint;
		int temp_int;
		BOrientedBox temp_box;

		UINT mesh_index = 0;
		if(this != nullptr)
			mesh_index= (m_mesh_count == 0) ? 0 : m_mesh_count - 1;
		auto& temp_data = mesh_data.at(mesh_index);

		while (ReadStringFromFile(rev_file, &prefix))
		{
			switch (HashCode(prefix.c_str()))
			{
			case HashCode("<Bounds>:"):
				//rev_file.read((char*)&temp_vec3, sizeof(Vec3));
				fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
				temp_box.SetCenter(temp_vec3);
				//rev_file.read((char*)&temp_vec3, sizeof(Vec3));
				fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
				temp_box.SetExtents(temp_vec3);
				temp_data.oriented_boxes.emplace_back(std::move(temp_box));
				break;

			case HashCode("<ControlPoints>:"):
				//rev_file.read((char*)&temp_uint, sizeof(UINT));

				fread(&temp_uint, sizeof(UINT), 1, rev_file);
				//vertex_count 정점수를 받는 변수 저장은 하지 않음
				// 벡터 positions 크기와 같으니까

				if (temp_uint > 0)
				{
					//rev_file.read((char*)&temp_vec3, sizeof(Vec3));
					for (UINT i = 0; i < temp_uint; ++i)
					{
						fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
						temp_data.positions.emplace_back(std::move(temp_vec3));
					}
				}
				break;

			case HashCode("<TextureCoords0>:"):
				//rev_file.read((char*)&temp_uint, sizeof(UINT));
				fread(&temp_uint, sizeof(UINT), 1, rev_file);
				//tex_coord_count
				if (temp_uint > 0)
				{
					//rev_file.read((char*)&temp_vec2, sizeof(Vec2));
					for (UINT i = 0; i < temp_uint; ++i)
					{
						fread(&temp_vec2, sizeof(Vec2), 1, rev_file);

						temp_data.tex_coords.emplace_back(std::move(temp_vec2));
					}
				}

				break;

			case HashCode("<TextureCoords1>:"):
				fread(&temp_uint, sizeof(UINT), 1, rev_file);
				//tex_coord2_count
				if (temp_uint > 0)
				{
					for (UINT i = 0; i < temp_uint; ++i)
					{
						fread(&temp_vec2, sizeof(Vec2), 1, rev_file);
						// temp_data.tex_coords2.emplace_back(std::move(temp_vec2));
					}
				}
				break;

			case HashCode("<Normals>:"):
				//rev_file.read((char*)&temp_uint, sizeof(UINT));
				fread(&temp_uint, sizeof(UINT), 1, rev_file);
				//normal_count
				if (temp_uint > 0)
				{
					for (UINT i = 0; i < temp_uint; ++i)
					{
						fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
						//rev_file.read((char*)&temp_vec3, sizeof(Vec3));
						temp_data.normals.emplace_back(std::move(temp_vec3));
					}
				}
				break;

			case HashCode("<Tangents>:"):
				fread(&temp_uint, sizeof(UINT), 1, rev_file);
				//rev_file.read((char*)&temp_uint, sizeof(UINT));
				//tan_count
				if (temp_uint > 0)
				{
					for (UINT i = 0; i < temp_uint; ++i)
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
					for (UINT i = 0; i < temp_uint; ++i)
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
					if (prefix.compare("</Polygons>") == 0)break;
					switch (HashCode(prefix.c_str()))
					{
					case HashCode("<SubIndices>:"):
					{
						fread(&temp_int, sizeof(int), 1, rev_file); //index_count
						//rev_file.read((char*)&temp_int, sizeof(int));

						fread(&temp_int, sizeof(int), 1, rev_file); //
						int sub_mesh_count = temp_int;
						//rev_file.read((char*)&temp_int, sizeof(int));

						//if (sub_mesh_count == 0) sub_mesh_count = 1;

						while (ReadStringFromFile(rev_file, &prefix))
						{
							if (prefix.compare("</Polygons>") == 0) break;
							switch (HashCode(prefix.c_str()))
							{
							case HashCode("<SubIndex>:"):
							{
								fread(&temp_int, sizeof(int), 1, rev_file); //index_count
								//rev_file.read((char*)&temp_int, sizeof(int));

								fread(&temp_int, sizeof(int), 1, rev_file);
								//rev_file.read((char*)&temp_int, sizeof(int));
								int subset_index_size = temp_int;
								if (subset_index_size > 0)
								{
									for (int i = 0; i < subset_index_size; i++)
									{
										fread(&temp_uint, sizeof(UINT), 1, rev_file);
										//rev_file.read((char*)&temp_uint, sizeof(UINT));
										temp_data.incdices.emplace_back(std::move(temp_uint));
									}
								}
							}
							break;
							default:
								break;
							}
						}
					}


					break;

					default:
						break;
					}
					if (prefix.compare("</Polygons>") == 0)break;

				}
				break;

			case HashCode("</Mesh>"):
				return;
			}
		}
	}

	void RevLoader::LoadSkinDeformations(FILE* rev_file, SPtr<BoneData>& bone_data) const
	{
		std::string prefix;

		Mat4 temp_mat4;
		Vec3 temp_vec3;
		Vec4 temp_vec4;
		IVec4 temp_ivec4;
		int temp_int;

		int skinning_bone_count = 0;
		int bone_per_vertex = 4;

		BOrientedBox temp_box;

		while (ReadStringFromFile(rev_file, &prefix))
		{
			if (prefix.compare("</SkinDeformations>") == 0) break;

			switch (HashCode(prefix.c_str()))
			{
			case HashCode("<BonesPerVertex>:"):
				fread(&bone_per_vertex, sizeof(int), 1, rev_file);
				//rev_file.read((char*)&bone_per_vertex, sizeof(int));
				break;
			case HashCode("<Bounds>:"):
				fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
				//rev_file.read((char*)&temp_vec3, sizeof(Vec3));
				temp_box.SetCenter(temp_vec3);
				fread(&temp_vec3, sizeof(Vec3), 1, rev_file);
				//rev_file.read((char*)&temp_vec3, sizeof(Vec3));
				temp_box.SetExtents(temp_vec3);
				bone_data->oriented_boxes.emplace_back(std::move(temp_box));
				//mesh->SetOrientBox(temp_box);
				break;
			case HashCode("<BoneNames>:"):
				fread(&skinning_bone_count, sizeof(int), 1, rev_file);
				//rev_file.read((char*)&skinning_bone_count, sizeof(int));
				if (skinning_bone_count > 0)
				{
					for (int i = 0; i < skinning_bone_count; ++i)
					{
						ReadStringFromFile(rev_file, &prefix);
						bone_data->bone_names.emplace_back(std::move(prefix));
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
						//temp_mat4.Transpose();
						bone_data->bone_offsets.emplace_back(std::move(temp_mat4));
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
						bone_data->bone_indices.emplace_back(std::move(temp_ivec4));
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
						bone_data->bone_weights.emplace_back(std::move(temp_vec4));
					}
				}
				break;
			default:
				break;
			}
		}
	}

	SPtr<AnimationSequence> RevLoader::LoadAnimation(FILE* file, const SPtr<Skeleton>& skeleton) const
	{
		SPtr<AnimationSequence> anim_seq = CreateSPtr<AnimationSequence>();
		std::string prefix;

		float start_time;
		float end_time;

		float weight;
		int animated_bone_count;
		std::vector<std::vector<SPtr<AnimationCurve>>> anim_curves;
		std::vector<SPtr<AnimationCurve>> curve;
		std::vector<SPtr<Skeleton>> cache_skeleton;

		int temp_int;

		ReadStringFromFile(file, &prefix); //"<AnimationSets>"
		fread(&temp_int, sizeof(int), 1, file); //animation_sets_count : 0

		ReadStringFromFile(file, &prefix); //"<AnimationSet>"
		fread(&temp_int, sizeof(int), 1, file); //animation_set_count : 1

		//AnimationSequence Load
		ReadStringFromFile(file, &(anim_seq->anim_name));

		fread(&start_time, sizeof(float), 1, file);
		fread(&end_time, sizeof(float), 1, file);


		ReadStringFromFile(file, &prefix); //"<AnimationLayers>"
		fread(&temp_int, sizeof(int), 1, file); //animation_layers_count : 1
		ReadStringFromFile(file, &prefix); //"<AnimationLayer>"
		fread(&temp_int, sizeof(int), 1, file); //animation_layer_count : 0

		//AnimationTrack
		fread(&animated_bone_count, sizeof(int), 1, file);
		fread(&weight, sizeof(float), 1, file);

		SPtr<AnimationTrack> anim_track = CreateSPtr<AnimationTrack>();
		anim_track->InitialIze(animated_bone_count, weight);


		for (int i = 0; i < animated_bone_count; ++i)
		{
			ReadStringFromFile(file, &prefix);
			switch (HashCode(prefix.c_str()))
			{
			case HashCode("<AnimationCurve>:"):
				fread(&temp_int, sizeof(int), 1, file); //curve_node_index == i

				ReadStringFromFile(file, &prefix); //bone_name
				auto& temp_skel = skeleton->FindBone(prefix);
				if (temp_skel)cache_skeleton.push_back(temp_skel);
				else
				{
					auto name_skel = prefix;
				}
				//AnimationCurve
				curve.resize(9);
				while (ReadStringFromFile(file, &prefix))
				{
					if (prefix.compare("</AnimationCurve>") == 0) break;

					switch (HashCode(prefix.c_str()))
					{ //0 : TX 1 : TY ... 8 : SZ 번호에 맞춰서
					case HashCode("<TX>:"):
						curve[0] = LoadKeyValue(file);
						break;
					case HashCode("<TY>:"):
						curve[1] = LoadKeyValue(file);
						break;
					case HashCode("<TZ>:"):
						curve[2] = LoadKeyValue(file);
						break;
					case HashCode("<RX>:"):
						curve[3] = LoadKeyValue(file);
						break;
					case HashCode("<RY>:"):
						curve[4] = LoadKeyValue(file);
						break;
					case HashCode("<RZ>:"):
						curve[5] = LoadKeyValue(file);
						break;
					case HashCode("<SX>:"):
						curve[6] = LoadKeyValue(file);
						break;
					case HashCode("<SY>:"):
						curve[7] = LoadKeyValue(file);
						break;
					case HashCode("<SZ>:"):
						curve[8] = LoadKeyValue(file);
						break;

					}
				}

				anim_curves.emplace_back(std::move(curve));
				//end
				break;
			}
		}
		anim_track->SetAnimationCurves(anim_curves);
		anim_track->SetCacheSkel(cache_skeleton);
		//end
		anim_seq->SetAnimationTrack(anim_track);
		anim_seq->SetDefaultTime(start_time, end_time);
		ReadStringFromFile(file, &prefix); //"</AnimationLayer>"
		ReadStringFromFile(file, &prefix); //"</AnimationLayers>"
		ReadStringFromFile(file, &prefix); //"<AnimationSet>"
		ReadStringFromFile(file, &prefix); //"<AnimationSets>"

		return anim_seq;
	}

	int RevLoader::ReadStringFromFile(FILE* file, std::string* word) const
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

	void RevLoader::AddRevMaterial(std::map<std::string, SPtr<Material>>&materials, const std::string& mtl_name, const std::string& parent_path) const
	{
		//std::map<std::string, SPtr<Material>> materials;

		bool is_new_mtl = false;
		SPtr<Material> material = nullptr;
		std::string texture_path = parent_path + "/" + mtl_name + ".png"; //rev 파일에서 확장자를 적도록 변경할예정

		auto AddMaterial([&material, &materials, &is_new_mtl]() {
			if (material != nullptr)
			{
				materials.insert({ material->GetName(), std::move(material) });
				material = nullptr;
			}
			is_new_mtl = false;
			});

		material = AssetStore::LoadMaterial(texture_path);
		if (material == nullptr)
		{
			AddMaterial();

			material = CreateSPtr<Material>();
			is_new_mtl = true;
			material->SetBaseColor(Vec4(0.3f, 0.3f, 0.3f, 1.0f)); //베이스컬러 회색

			material->SetAssetInfo({ mtl_name, texture_path, ".png" }); //텍스처 경로 = 마테리얼 경로(마테리얼 파일이없기때문에)
			SPtr<ExternalTexture> diffuse_texture = AssetStore::LoadTexture(texture_path);
			if (diffuse_texture != nullptr)
				material->SetDiffuseTexture(diffuse_texture);
			AddMaterial();

		}
		//return materials;
	}
	void RevLoader::InitializeMeshData(std::vector<MeshData>& mesh_data) const
	{
		mesh_data.emplace_back(std::move(MeshData{}));
		mesh_data.back().bone_data = CreateSPtr<BoneData>();
	}
}
