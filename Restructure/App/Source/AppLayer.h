
#pragma once

#include <stdint.h>
#include <memory>
#include "Core/Layer.h"
#include "Core/Window.h"
#include "Core/InputManager.h"
#include "Core/Renderer/Camera.h"
class AppLayer : public Core::Layer
{
public:
    AppLayer();
    virtual ~AppLayer();

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;
private:
    uint32_t m_Shader = 0;
    uint32_t m_VertexArray = 0;
    uint32_t m_VertexBuffer = 0;
    std::shared_ptr<Core::Window> m_Window = nullptr;
    std::shared_ptr<Camera> p_Camera = nullptr;
    std::shared_ptr<Core::InputManager> m_InputManager = nullptr;
};
