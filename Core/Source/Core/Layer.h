#pragma once

#include "Event.h"

namespace Core
{

	class Layer
	{
	public:
		enum class State
		{
			Active,
			Suspended_Update,
			Suspended_UpdateAndRender,
		};

	public:
		virtual ~Layer() = default;

		virtual void OnEvent(Event &event) {}

		virtual void OnUpdate(float ts) {}
		virtual void OnRender() {}

		void SuspendUpdate() { m_State = State::Suspended_Update; }
		void SuspendUpdateAndRender() { m_State = State::Suspended_UpdateAndRender; }
		void Resume() { m_State = State::Active; }

		State GetState() const { return m_State; }

	protected:
		State m_State = State::Active;
	};

}