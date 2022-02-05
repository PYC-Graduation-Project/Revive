#include "stdafx.h"
#include "client/asset/mesh/mesh_loader.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/mesh/vertex.h"

namespace client_fw
{
	SPtr<Mesh> MeshLoader::LoadMesh(const std::string& name, const std::string& path, const std::string& extension)
	{
		SPtr<Mesh> mesh = nullptr;

		switch (HashCode(extension.c_str()))
		{
		case HashCode(".obj"):
			mesh = LoadObj(name, path, extension);
			break;
		case HashCode(".rev"):
			mesh = LoadRev(name, path, extension); //이거를 void로햇어야햇나
			break;
		default:
			LOG_ERROR("Files in {0} format cannot be supported", extension);
			break;
		}

		return mesh;
	}

	struct CombineData
	{
		std::string name; //추후에 Material로?
		std::vector<UINT> pos_indices;
		std::vector<UINT> tex_indices;
		std::vector<UINT> normal_indices;
	};

	SPtr<StaticMesh> MeshLoader::LoadObj(const std::string& name, const std::string& path, const std::string& extension)
	{
		std::ifstream obj_file(path);
		
		if (obj_file.is_open() == false)
		{
			LOG_ERROR("Could not find path : [{0}]", path);
			return nullptr;
		}

		std::vector<Vec3> positions;
		std::vector<Vec3> normals;
		std::vector<Vec2> tex_coords;
		std::vector<CombineData> combine_data;
		UINT combine_data_index = 0;

		std::stringstream ss;
		std::string line;
		std::string prefix;

		Vec3 temp_vec;
		UINT temp_uint = 0;
		std::string temp_string;

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

		while (std::getline(obj_file, line))
		{
			ss.clear();
			ss.str(line);
			ss >> prefix;

			switch (HashCode(prefix.c_str()))
			{
			case HashCode("v"):
				ss >> temp_vec.x >> temp_vec.y >> temp_vec.z;
				temp_vec.z *= -1.0f;
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
							return data.name == temp_string;
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
						else if(count == 1)
							combine_data[combine_data_index].tex_indices.emplace_back(temp_uint - 1);
						else if(count == 2)
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

		SPtr<StaticMesh> mesh = CreateSPtr<StaticMesh>();

		UINT vertex_count = 0;
		for (const auto& data : combine_data)
			vertex_count += static_cast<UINT>(data.pos_indices.size());

		std::vector<TextureLightVertex> vertices(vertex_count);

		vertex_count = 0;
		for (const auto& data : combine_data)
		{
			UINT count = static_cast<UINT>(data.pos_indices.size());
			mesh->AddInstanceInfo({ count, vertex_count });

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
		if (FAILED(D3DCreateBlob(vertices_size, &mesh->GetVertexBufferBlob())))
		{
			LOG_ERROR("Could not create blob for vertex");
			return nullptr;
		}
		CopyMemory(mesh->GetVertexBufferBlob()->GetBufferPointer(), vertices.data(), vertices_size);

		Vec3 center = max_pos + min_pos;
		Vec3 extents = (max_pos - min_pos) * 0.5f;
		extents.x = abs(extents.x), extents.y = abs(extents.y), extents.z = abs(extents.z);
		BoundingOrientedBox oriented_box;
		oriented_box.Center = center;
		oriented_box.Extents = extents;

		mesh->SetOrientBox(oriented_box);

		return mesh;
	}

	SPtr<SkeletalMesh> MeshLoader::LoadRev(const std::string& name, const std::string& path, const std::string& extension)
	{
		std::ifstream rev_file(path);

		if (rev_file.is_open() == false)
		{
			LOG_ERROR("Could not find path : [{0}]", path);
			return nullptr;
		}

		std::vector<Vec3> positions;
		std::vector<Vec3> normals;
		std::vector<Vec2> tex_coords;
		std::vector<CombineData> combine_data;
		UINT combine_data_index = 0;

		std::stringstream ss;
		std::string line;
		std::string prefix;

		Vec3 temp_vec;
		UINT temp_uint = 0;
		std::string temp_string;

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
		
		while (ReadFromFile(rev_file,&ss))
		{
			ss >> prefix;

			switch (HashCode(prefix.c_str()))
			{
			case HashCode("<Hierarchy>"):
				LoadFrameHierArchy(rev_file,skeleton,s_mesh);
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
	void MeshLoader::LoadFrameHierArchy(std::ifstream &rev_file, SPtr<Skeleton>& skeleton, SPtr<SkeletalMesh>& mesh)
	{
		std::stringstream ss;
		std::string line;
		std::string prefix;
		
		std::vector<Mat4> parent;
		std::vector<Vec3> scale;
		std::vector<Vec3> rotation;
		std::vector<Vec3> translation;

		int n_frame = 0; //프레임 수
		int n_childs = 0;

		std::string b_name;
		Mat4 temp_mat4;
		Vec3 temp_vec3;

		while (ReadFromFile(rev_file,&ss))
		{
			
			ss >> prefix;
			switch (HashCode(prefix.c_str()))
			{
			case HashCode("<Frame>:"):
				rev_file.read((char*)&n_frame, sizeof(int));

				ReadFromFile(rev_file, &ss); //bone name read
				ss >> b_name;
				skeleton->SetBoneName(b_name);

				while (ReadFromFile(rev_file, &ss))
				{
					ss >> prefix;
					switch (HashCode(prefix.c_str()))
					{
					case HashCode("<Transform>:"):
						rev_file.read((char*)&temp_mat4, sizeof(Mat4));
						parent.emplace_back(std::move(temp_mat4));

						rev_file.read((char*)&temp_vec3, sizeof(Vec3));
						scale.emplace_back(std::move(temp_vec3));
						rev_file.read((char*)&temp_vec3, sizeof(Vec3));
						rotation.emplace_back(std::move(temp_vec3));
						rev_file.read((char*)&temp_vec3, sizeof(Vec3));
						translation.emplace_back(std::move(temp_vec3));

						break;
						
					case HashCode("<Mesh>:"):
						LoadMeshFromFIle(rev_file);
						break; 
						
					case HashCode("<SkinDeformations>:"):
						break;
						
					case HashCode("<Materals>:"):
						break;
						
					case HashCode("<Children>:"):
						rev_file.read((char*)&n_childs, sizeof(int));
						for (int i = 0; i < n_childs; ++i)
						{
							SPtr<Skeleton> child = nullptr;
							LoadFrameHierArchy(rev_file,child,mesh);
							if (child) skeleton->SetChild(child);
						}
						break;
						
					case HashCode("</Frame>:"):
						break;

					}
				}
				break;
			case HashCode("</Hierarchy>"):
				break;
			}
		}
	}
	void MeshLoader::LoadMeshFromFIle(std::ifstream& rev_file)
	{
		std::string mesh_name;
		client_fw::ReadStringFromFile(rev_file, &mesh_name);

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
				rev_file.read((char*)&temp_vec3, sizeof(Vec3));
				oriented_box.Center = temp_vec3;
				rev_file.read((char*)&temp_vec3, sizeof(Vec3));
				oriented_box.Extents = temp_vec3;
				break;

			case HashCode("<ControlPoints>:"):
				rev_file.read((char*)&temp_uint, sizeof(UINT));
				//vertex_count 정점수를 받는 변수 저장은 하지 않음
				// 벡터 positions 크기와 같으니까
				if (temp_uint > 0)
				{
					rev_file.read((char*)&temp_vec3, sizeof(Vec3));
					//temp_vec.z *= -1.0f; 필요할까?
					positions.emplace_back(std::move(temp_vec3));
				}
				break;

			case HashCode("<TextureCoords0>:"):
				rev_file.read((char*)&temp_uint, sizeof(UINT));
				//tex_coord_count
				if (temp_uint > 0)
				{
					rev_file.read((char*)&temp_vec2, sizeof(Vec2));
					tex_coords.emplace_back(std::move(temp_vec2));
				}

				break;

			case HashCode("<TextureCoords1>:"):
				break;

			case HashCode("<Normals>:"):
				rev_file.read((char*)&temp_uint, sizeof(UINT));
				//normal_count
				if (temp_uint > 0)
				{
					rev_file.read((char*)&temp_vec3, sizeof(Vec3));
					normals.emplace_back(std::move(temp_vec3));
				}
				break;

			case HashCode("<Tangents>:"):
				rev_file.read((char*)&temp_uint, sizeof(UINT));
				//tan_count
				if (temp_uint > 0)
				{
					rev_file.read((char*)&temp_vec3, sizeof(Vec3));
					//tangent.emplace_back
				}
				break;

			case HashCode("<BiTangents>:"):
				rev_file.read((char*)&temp_uint, sizeof(UINT));
				//bitan_count
				if (temp_uint > 0)
				{
					rev_file.read((char*)&temp_vec3, sizeof(Vec3));
					//bitangent.emplace_back
				}
				break;

			case HashCode("<Polygons>:"):
				rev_file.read((char*)&temp_int, sizeof(int));
				//Polgon_count
				while (ReadStringFromFile(rev_file, &prefix))
				{
					switch (HashCode(prefix.c_str()))
					{
					case HashCode("<SubIndices>:"):
						rev_file.read((char*)&temp_int, sizeof(int));
						//index_count
						rev_file.read((char*)&temp_int, sizeof(int));
						//sub_mesh_count = temp_int;
						//if (sub_mesh_count == 0) sub_mesh_count = 1;
						
						while (ReadStringFromFile(rev_file, &prefix))
						{
							switch (HashCode(prefix.c_str()))
							{
							case HashCode("<SubIndex>:"):
								rev_file.read((char*)&temp_int, sizeof(int));
								//index_count
								rev_file.read((char*)&temp_int, sizeof(int));
								v_subset_index_count.emplace_back(std::move(temp_int));
								int subset_index_size = v_subset_index_count.back();
								if (subset_index_size > 0)
								{
									for (int i = 0; i < subset_index_size; i++)
									{
										rev_file.read((char*)&temp_uint, sizeof(UINT));
										temp_v_uint.emplace_back(std::move(temp_uint));
									}
									vv_subset_index_count.emplace_back(std::move(temp_v_uint));
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
				break;
			}
		}

		
	}

	int ReadFromFile(std::ifstream& file, std::stringstream* word)
	{
		word->clear();
		int length = 0;
		char buffer[64] = { "\0" };
		file.read((char *)&length, sizeof(int));
		file.read(buffer, sizeof(char) * length);
		word->str(buffer);
		return length;
	}
	int ReadStringFromFile(std::ifstream& file, std::string* word)
	{
		word->clear();
		int length = 0;
		char buffer[64] = { "\0" };
		file.read((char*)&length, sizeof(int));
		file.read(buffer, sizeof(char) * length);
		*word = buffer;
		return length;
	}
}
