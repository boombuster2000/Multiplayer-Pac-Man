#pragma once

#include "Layer.h"
#include <vector>
#include <string>
#include <memory>

namespace Core {

	struct ApplicationSpecification
	{
		std::string Name = "Raylib Application";

        uint32_t Width = 1280;
        uint32_t Height = 720;
        int targetFPS = 60;
	};

	class Application
	{
    private:
        ApplicationSpecification m_Specification;
        bool m_Running = false;

        std::vector<std::unique_ptr<Layer>> m_LayerStack;

	public:
		Application(const ApplicationSpecification& specification = ApplicationSpecification());
		~Application();

		void Run();
		void Stop();

		template<typename TLayer>
		void PushLayer()
		{
			static_assert(std::is_base_of_v<Layer, TLayer>, "TLayer must derive from Layer");
			m_LayerStack.push_back(std::make_unique<TLayer>());
		}


		static Application& Get();
        
	};

}