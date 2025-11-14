#include "Application.h"
#include "InputManager.h"
#include <iostream>
#include <raylib.h>
#include <cassert>
#include <algorithm>

namespace Core
{

	static Application *s_Application = nullptr;
	static std::shared_ptr<TexturesManager> s_TexturesManager = nullptr;
	static std::shared_ptr<InputManager> s_InputManager = nullptr;
	std::queue<LayerAction> Application::s_PendingActions;

	Application::Application(const ApplicationSpecification &specification)
		: m_Specification(specification)
	{
		s_Application = this;
		s_TexturesManager = std::make_shared<TexturesManager>();
		s_InputManager = std::make_shared<InputManager>();

		// Keyboard
		s_InputManager->AddKeyboardAction("move_up", KEY_W);
		s_InputManager->AddKeyboardAction("move_up", KEY_UP);
		s_InputManager->AddKeyboardAction("move_down", KEY_S);
		s_InputManager->AddKeyboardAction("move_down", KEY_DOWN);
		s_InputManager->AddKeyboardAction("move_left", KEY_A);
		s_InputManager->AddKeyboardAction("move_left", KEY_LEFT);
		s_InputManager->AddKeyboardAction("move_right", KEY_D);
		s_InputManager->AddKeyboardAction("move_right", KEY_RIGHT);
		s_InputManager->AddKeyboardAction("confirm", KEY_ENTER);
		s_InputManager->AddKeyboardAction("quit", KEY_Q);

		// Gamepad
		s_InputManager->AddGamepadButtonAction("move_up", GAMEPAD_BUTTON_LEFT_FACE_UP, 0);
		s_InputManager->AddGamepadButtonAction("move_down", GAMEPAD_BUTTON_LEFT_FACE_DOWN, 0);
		s_InputManager->AddGamepadButtonAction("move_left", GAMEPAD_BUTTON_LEFT_FACE_LEFT, 0);
		s_InputManager->AddGamepadButtonAction("move_right", GAMEPAD_BUTTON_LEFT_FACE_RIGHT, 0);
		s_InputManager->AddGamepadButtonAction("confirm", GAMEPAD_BUTTON_RIGHT_FACE_DOWN, 0);

		s_InputManager->AddGamepadAxisAction("move_up", GAMEPAD_AXIS_LEFT_Y, 0, false);
		s_InputManager->AddGamepadAxisAction("move_down", GAMEPAD_AXIS_LEFT_Y, 0, true);
		s_InputManager->AddGamepadAxisAction("move_left", GAMEPAD_AXIS_LEFT_X, 0, false);
		s_InputManager->AddGamepadAxisAction("move_right", GAMEPAD_AXIS_LEFT_X, 0, true);

		InitWindow(specification.Width, specification.Height, specification.Name.c_str());
		SetExitKey(specification.programExitKey);

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

	std::shared_ptr<InputManager> Application::GetInputManager()
	{
		return s_InputManager;
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

			s_InputManager->Update();

			ProcessPendingActions();

			// Main layer update here
			for (const std::unique_ptr<Layer> &layer : m_LayerStack)
			{
				if (layer->GetState() == Layer::State::Active)
					layer->OnUpdate(GetFrameTime());
			}

			BeginDrawing();

			// NOTE: rendering can be done elsewhere (eg. render thread)
			for (const std::unique_ptr<Layer> &layer : m_LayerStack)
			{
				if (layer->GetState() != Layer::State::Suspended_UpdateAndRender)
					layer->OnRender();
			}

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