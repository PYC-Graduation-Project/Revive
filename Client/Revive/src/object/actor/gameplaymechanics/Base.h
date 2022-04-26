#pragma once
#include "object/actor/wall.h"

namespace revive
{
	class Base :public Wall
	{
	public:
		Base(const std::vector<SPtr<StaticMeshComponent>>& static_mesh_components, const std::vector<SPtr<BoxComponent>>& box_components);
		virtual ~Base() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;
		//virtual void Shutdown() override;
		virtual void ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message) override;

	private:
		int m_hp;
		int m_network_id = -1;
	public:
		const int GetID() const { return m_network_id; }
		void SetID(const int hp) { m_hp = hp; }

		const int GetHP() const { return m_hp; }
		void SetHP(const int hp) { m_hp = hp; }
	};

}