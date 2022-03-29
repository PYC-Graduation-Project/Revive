#pragma once

namespace client_fw
{
	enum class eSkyType;

	class SkyComponent;
	class SkyCubeComponent;
	class SkyCubeMesh;
	class SkySphereComponent;

	class SkyRenderItem final
	{
	public:
		SkyRenderItem(const std::string& owner_shader_name);
		virtual ~SkyRenderItem();

		void Initialize(ID3D12Device* device);
		void Shutdown();

		void Update(ID3D12Device* device);
		void UpdateFrameResource(ID3D12Device* device);
		void Draw(ID3D12GraphicsCommandList* command_list,
			std::function<void()>&& cube_draw_function,
			std::function<void()>&& sphere_draw_function);

		void RegisterSkyComponent(const SPtr<SkyComponent>& sky_component);
		void UnregisterSkyComponent(const SPtr<SkyComponent>& sky_component);

	private:
		std::string m_owner_shader_name;

		eSkyType m_draw_sky_type;
		std::vector<SPtr<SkyComponent>> m_sky_components;

		SPtr<SkyCubeMesh> m_sky_cube_mesh;
	};

}


