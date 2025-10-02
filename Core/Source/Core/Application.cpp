#include "Application.h"
#include <iostream>
#include <raylib.h>
#include <cassert>

namespace Core {

	static Application* s_Application = nullptr;

	static void GLFWErrorCallback(int error, const char* description)
	{
		std::cerr << "[GLFW Error]: " << description << std::endl;
	}

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		s_Application = this;

		InitWindow(specification.Width, specification.Height, specification.Name.c_str());
        SetTargetFPS(specification.targetFPS);
	}

	Application::~Application()
	{
		CloseWindow();
		s_Application = nullptr;
	}

	void Application::Run()
	{
		m_Running = true;

		float lastTime = GetTime();

		// Main Application loop
		while (m_Running)
		{
			if (WindowShouldClose())
			{
				Stop();
				break;
			}
    

			// Main layer update here
			for (const std::unique_ptr<Layer>& layer : m_LayerStack)
				layer->OnUpdate(GetFrameTime());

            BeginDrawing();

			// NOTE: rendering can be done elsewhere (eg. render thread)
			for (const std::unique_ptr<Layer>& layer : m_LayerStack)
				layer->OnRender();

            EndDrawing();
		}
	}

	void Application::Stop()
	{
		m_Running = false;
	}


	Application& Application::Get()
	{
		assert(s_Application);
		return *s_Application;
	}


}