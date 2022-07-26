#pragma once
#include "object/actor/visual_actor.h"

namespace revive
{
	class Base :public VisualActor
	{
	public:
		Base();
		Base(const std::vector<SPtr<StaticMeshComponent>>& static_mesh_components, const std::vector<SPtr<BoxComponent>>& box_components);
		virtual ~Base() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;
		virtual void Shutdown() override;
		virtual void ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message) override;

	private:
		float m_hp = 100;
		float m_max_hp;
		int m_network_id = -1;

		std::function<void(float, float)> m_changed_hp_function;

	public:
		const int GetID() const { return m_network_id; }
		void SetID(const int id) { m_network_id = id; }

		const float GetHP() const { return m_hp; }
		void SetHP(const float hp) { m_hp = hp; }

		const float GetMaxHP() const { return m_max_hp; }
		void SetMaxHP(const float hp) { m_max_hp = hp; }

		void OnChangeHPFunction(std::function<void(float, float)>&& function) { m_changed_hp_function = function; }
	};

}