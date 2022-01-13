#pragma once
#include "client/object/actor/core/actor.h"
#include "client/object/actor/controller.h"

namespace client_fw
{
	class Controller;

	class Pawn : public Actor
	{
	public:
		Pawn(const std::string& name = "pawn");
		virtual ~Pawn() = default;

		virtual void AddMovementInput(const Vec3& direction, float scale) {}

		friend void Controller::Possess(const SPtr<Pawn>& pawn);

	protected:
		virtual void RegisterPressedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption = true);
		virtual void RegisterReleasedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption = true);
		virtual void RegisterAxisEvent(const std::string& name, std::vector <AxisEventKeyInfo>&& keys,
			const std::function<bool(float)>& func, bool consumption = true);

	private:
		WPtr<Controller> m_controller;

	private:
		void SetController(const SPtr<Controller>& controller);
	};
}



