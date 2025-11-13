#pragma once

#include "Layer.h"
#include "TexturesManager.h"
#include "InputManager.h"
#include <vector>
#include <string>
#include <memory>
#include <queue>
#include <functional>
#include <typeindex>

namespace Core
{
	class InputManager;

	struct ApplicationSpecification
	{
		std::string Name = "Raylib Application";

		uint32_t Width = 1280;
		uint32_t Height = 720;
		int targetFPS = 60;
		bool drawFPS = true;
	};

	enum class LayerActionType
	{
		Push,
		Pop,
		Transition
	};

	struct LayerAction
	{
		LayerActionType Type;
		std::type_index FromType = std::type_index(typeid(void));
		std::function<std::unique_ptr<Layer>()> ToFactory = nullptr;
	};

	class Application
	{
	private:
		ApplicationSpecification m_Specification;
		bool m_Running = false;

		std::vector<std::unique_ptr<Layer>> m_LayerStack;

		static std::queue<LayerAction> s_PendingActions;

		void ProcessPendingActions();

	public:
		Application(const ApplicationSpecification &specification = ApplicationSpecification());
		~Application();

		static std::shared_ptr<TexturesManager> GetTexturesManager();
		static std::shared_ptr<InputManager> GetInputManager();

		void Run();
		void Stop();

		template <typename TLayer>
		static void QueuePush()
		{
			static_assert(std::is_base_of_v<Layer, TLayer>, "TLayer must derive from Layer");
			s_PendingActions.push({LayerActionType::Push,
								   std::type_index(typeid(void)),
								   []()
								   { return std::make_unique<TLayer>(); }});
		}

		template <typename TLayer>
		static void QueuePop()
		{
			static_assert(std::is_base_of_v<Layer, TLayer>, "TLayer must derive from Layer");
			s_PendingActions.push({LayerActionType::Pop, std::type_index(typeid(TLayer)), nullptr});
		}

		template <typename TFrom, typename TTo>
		static void QueueTransition()
		{
			static_assert(std::is_base_of_v<Layer, TFrom>, "TFrom must derive from Layer");
			static_assert(std::is_base_of_v<Layer, TTo>, "TTo must derive from Layer");

			s_PendingActions.push({LayerActionType::Transition,
								   std::type_index(typeid(TFrom)),
								   []()
								   { return std::make_unique<TTo>(); }});
		}

		static Application &Get();
	};

}