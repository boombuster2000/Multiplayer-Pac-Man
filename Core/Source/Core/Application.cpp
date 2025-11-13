#include "Application.h"
#include <iostream>
#include <raylib.h>
#include <cassert>
#include <algorithm>

namespace Core
{

	static Application *s_Application = nullptr;
	static std::shared_ptr<TexturesManager> s_TexturesManager = nullptr;
	std::queue<LayerAction> Application::s_PendingActions;

	Application::Application(const ApplicationSpecification &specification)
		: m_Specification(specification)
	{
		s_Application = this;
		s_TexturesManager = std::make_shared<TexturesManager>();

		InitWindow(specification.Width, specification.Height, specification.Name.c_str());
		if (specification.targetFPS > 0)
			SetTargetFPS(specification.targetFPS);
	}

	Application::~Application()
	{
		CloseWindow();
		s_Application = nullptr;
	}

	std::shared_ptr<TexturesManager> Application::GetTexturesManager()
	{
		return s_TexturesManager;
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

			ProcessPendingActions();

			// Main layer update here
			for (const std::unique_ptr<Layer> &layer : m_LayerStack)
				layer->OnUpdate(GetFrameTime());

			BeginDrawing();

			// NOTE: rendering can be done elsewhere (eg. render thread)
			for (const std::unique_ptr<Layer> &layer : m_LayerStack)
				layer->OnRender();

			if (m_Specification.drawFPS)
				DrawFPS(GetScreenWidth() - 100, 10);

			EndDrawing();
		}
	}

	void Application::Stop()
	{
		m_Running = false;
	}

	void Application::ProcessPendingActions()
	{
		while (!s_PendingActions.empty())
		{
			LayerAction action = s_PendingActions.front();
			s_PendingActions.pop();

			switch (action.Type)
			{
			case LayerActionType::Push:
				m_LayerStack.push_back(action.ToFactory());
				break;

			case LayerActionType::Pop:
			{
				auto it = std::find_if(m_LayerStack.begin(), m_LayerStack.end(),
								   [&](const std::unique_ptr<Layer> &layer)
								   {
									   return std::type_index(typeid(*layer)) == action.FromType;
								   });
				if (it != m_LayerStack.end())
					m_LayerStack.erase(it);
			}
			break;

			case LayerActionType::Transition:
			{
				auto it = std::find_if(m_LayerStack.begin(), m_LayerStack.end(),
								   [&](const std::unique_ptr<Layer> &layer)
								   {
									   return std::type_index(typeid(*layer)) == action.FromType;
								   });
				if (it != m_LayerStack.end())
					*it = action.ToFactory();
			}
			break;
			}
		}
	}

	Application &Application::Get()
	{
		assert(s_Application);
		return *s_Application;
	}

}