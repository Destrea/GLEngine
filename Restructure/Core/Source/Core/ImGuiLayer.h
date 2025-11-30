#pragma once


//#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "Layer.h"
#include "Window.h"
#include "InputEvents.h"
#include "WindowEvents.h"
#include "Application.h"

#include <memory>
#include <GLFW/glfw3.h>
#include <glad/glad.h>


class ImGuiLayer : public Core::Layer
{
public:
    ImGuiLayer();
    virtual ~ImGuiLayer();

    virtual void OnEvent(Core::Event& event) override;

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;

    void BeginEngineUI();
    void EndEngineUI();
    void toggleHidden();
    void GetFPS();
private:
    bool OnMouseButtonPressed(Core::MouseButtonPressedEvent& event);
    bool OnMouseMoved(Core::MouseMovedEvent& event);
    bool OnWindowClosed(Core::WindowClosedEvent& event);
    bool OnKeyPressed(Core::KeyPressedEvent& event);


    GLFWwindow* window = nullptr;
    std::shared_ptr<Core::Window> m_Window = nullptr;
    ImGuiIO m_io;
    bool m_IsHidden = false;
    float m_Timestep;
    float m_Time = 0.0f;
};
