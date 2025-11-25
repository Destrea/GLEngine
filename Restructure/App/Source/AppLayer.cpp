
#include "AppLayer.h"

#include "Core/Application.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Shader.h"
#include "Core/InputManager.h"
#include "Core/ResourceManager.h"
#include "Core/Renderer/Camera.h"
#include <glm/glm.hpp>

#include <print>

AppLayer::AppLayer()
{
    //Create Shader and load it.
    Renderer::Shader shader = Core::ResourceManager::LoadShader("Shaders/Vertex.glsl", "Shaders/Fragment.glsl", "test");

    //m_Shader = newShader.CreateGraphicsShader("Shaders/Vertex.glsl", "Shaders/Fragment.glsl");
    //newShader.ID = m_Shader;
    p_Camera = std::make_shared<Camera>(glm::vec3(0.0f,0.0f,3.0f));
    m_InputManager = std::make_shared<Core::InputManager>();
    // Create geometry
    glCreateVertexArrays(1, &m_VertexArray);
    glCreateBuffers(1, &m_VertexBuffer);

    struct Vertex
    {
        glm::vec2 Position;
        glm::vec2 TexCoord;
    };

    Vertex vertices[] = {
        { {-1.0f, -1.0f }, { 0.0f, 0.0f } },  // Bottom-left
        { { 3.0f, -1.0f }, { 2.0f, 0.0f } },  // Bottom-right
        { {-1.0f,  3.0f }, { 0.0f, 2.0f } }   // Top-left
    };

    glNamedBufferData(m_VertexBuffer, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind the VBO to VAO at binding index 0
    glVertexArrayVertexBuffer(m_VertexArray, 0, m_VertexBuffer, 0, sizeof(Vertex));

    // Enable attributes
    glEnableVertexArrayAttrib(m_VertexArray, 0); // position
    glEnableVertexArrayAttrib(m_VertexArray, 1); // uv

    // Format: location, size, type, normalized, relative offset
    glVertexArrayAttribFormat(m_VertexArray, 0, 2, GL_FLOAT, GL_FALSE, static_cast<GLuint>(offsetof(Vertex, Position)));
    glVertexArrayAttribFormat(m_VertexArray, 1, 2, GL_FLOAT, GL_FALSE, static_cast<GLuint>(offsetof(Vertex, TexCoord)));

    // Link attribute locations to binding index 0
    glVertexArrayAttribBinding(m_VertexArray, 0, 0);
    glVertexArrayAttribBinding(m_VertexArray, 1, 0);
}

AppLayer::~AppLayer()
{
    glDeleteVertexArrays(1, &m_VertexArray);
    glDeleteBuffers(1, &m_VertexBuffer);

    glDeleteProgram(m_Shader);
}


void AppLayer::OnEvent(Core::Event& event)
{
    std::println("{}", event.ToString());

    Core::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) {return OnMouseButtonPressed(e); });

    dispatcher.Dispatch<Core::MouseMovedEvent>([this](Core::MouseMovedEvent& e) {return OnMouseMoved(e);});

    dispatcher.Dispatch<Core::WindowClosedEvent>([this](Core::WindowClosedEvent& e) {return OnWindowClosed(e); });

    dispatcher.Dispatch<Core::KeyPressedEvent>([this](Core::KeyPressedEvent& e) {return OnKeyPressed(e); });
}


void AppLayer::OnUpdate(float ts)
{
    //m_Time += ts;

    if(glfwGetKey(Core::Application::Get().GetWindow()->GetHandle(), GLFW_KEY_1) == GLFW_PRESS)
    {
        //Hot-reload shader
    }
}

void AppLayer::OnRender()
{
    Renderer::Shader newShader = Core::ResourceManager::GetShader("test");
    newShader.Use();

    glm::mat4 projection = glm::perspective(glm::radians(p_Camera->Zoom), (float)1920 / (float)1080, 0.1f, 100.0f);
    glm::mat4 view = p_Camera->GetViewMatrix();

    newShader.setMatrix4("projection", projection);
    newShader.setMatrix4("view",view);

    glm::mat4 model = glm::mat4(1.0f);
    newShader.setMatrix4("model", model);


    // Uniforms
    glUniform1f(0, Core::Application::GetTime());

    glm::vec2 framebufferSize = Core::Application::Get().GetFramebufferSize();
    glUniform2f(1, framebufferSize.x, framebufferSize.y);

    glViewport(0, 0, static_cast<GLsizei>(framebufferSize.x), static_cast<GLsizei>(framebufferSize.y));

    // Render
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(m_VertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}





bool AppLayer::OnMouseButtonPressed(Core::MouseButtonPressedEvent& event)
{

    //Mouse button press handling.
    //Double check with TheCherno github to see how its used.
    return false;
}

bool AppLayer::OnMouseMoved(Core::MouseMovedEvent& event)
{
    //Mouse movement handling.
    //m_MousePosition = {static_cast<float>(event.GetX()), static_cast<float>(event.GetY()) };

    return false;
}


bool AppLayer::OnWindowClosed(Core::WindowClosedEvent& event)
{
    //Window closing Handling.
    std::println("Window Closed!");

    return false;
}

bool AppLayer::OnKeyPressed(Core::KeyPressedEvent& event)
{
    //Keyboard Input handling ONLY for AppLayer
    if(event.GetKeyCode() == GLFW_KEY_J)
    {
        printf("\n\nPoggers!\n\n");
    }

    return false;
}




