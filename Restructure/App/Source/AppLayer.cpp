
#include "AppLayer.h"

#include "Core/Application.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Shader.h"
#include "Core/InputManager.h"
#include "Core/ResourceManager.h"
#include "Core/Renderer/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <print>

AppLayer::AppLayer()
{
    //Create Shader and load it.
    Renderer::Shader shader = Core::ResourceManager::LoadShader("Shaders/Vertex.glsl", "Shaders/Frag.glsl", "test");
    Renderer::Texture2D testTex = Core::ResourceManager::LoadTexture("Textures/wall.jpg", false, "wall");
    //m_GuiLayer = std::make_shared<ImGuiLayer>();

    p_Camera = std::make_shared<Camera>(glm::vec3(0.0f,0.0f,3.0f));
    m_InputManager = std::make_shared<Core::InputManager>();
    //Disable cursor by default.
    glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // Create geometry
    glCreateVertexArrays(1, &m_VertexArray);
    glCreateBuffers(1, &m_VertexBuffer);

    struct Vertex
    {
        glm::vec2 Position;
        glm::vec2 TexCoord;
    };

    Vertex vertices[] = {
        // Vertex Coord     Tex Coord
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

    Renderer::Texture2D tex = Core::ResourceManager::GetTexture("wall");
    shader.setInteger("texture1", tex.ID);

}

AppLayer::~AppLayer()
{
    glDeleteVertexArrays(1, &m_VertexArray);
    glDeleteBuffers(1, &m_VertexBuffer);

    glDeleteProgram(m_Shader);
}


void AppLayer::OnEvent(Core::Event& event)
{
    //Debugging console output, tied to every event.
    //std::println("{}", event.ToString());

    Core::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) {return OnMouseButtonPressed(e); });

    dispatcher.Dispatch<Core::MouseMovedEvent>([this](Core::MouseMovedEvent& e) {return OnMouseMoved(e);});

    dispatcher.Dispatch<Core::WindowClosedEvent>([this](Core::WindowClosedEvent& e) {return OnWindowClosed(e); });

    dispatcher.Dispatch<Core::KeyPressedEvent>([this](Core::KeyPressedEvent& e) {return OnKeyPressed(e); });
}

//Handle physics or player Input. Things that need to be tied to framerate/game tick system for accuracy or fluidity.
void AppLayer::OnUpdate(float ts)
{
    //Keyboard Input
    m_InputManager->processKeyboardInput(Core::Application::Get().GetWindow()->GetHandle(), p_Camera, ts);
    m_InputManager->processMouseInput(p_Camera, m_MousePosition.x, m_MousePosition.y);
    //Mouse Input

}

void AppLayer::OnRender()
{
    Renderer::Shader newShader = Core::ResourceManager::GetShader("test");
    newShader.Use();
    Renderer::Texture2D tex = Core::ResourceManager::GetTexture("wall");


    glm::mat4 projection = glm::perspective(glm::radians(p_Camera->Zoom), (float)1920 / (float)1080, 0.1f, 100.0f);
    glm::mat4 view = p_Camera->GetViewMatrix();

    newShader.setMatrix4("projection", projection);
    newShader.setMatrix4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    newShader.setMatrix4("model", model);


    //Uniforms
    //glUniform1f(0, Core::Application::GetTime());

    //glm::vec2 framebufferSize = Core::Application::Get().GetFramebufferSize();
    //glUniform2f(1, framebufferSize.x, framebufferSize.y);

    //glViewport(0, 0, static_cast<GLsizei>(framebufferSize.x), static_cast<GLsizei>(framebufferSize.y));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex.ID);



    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // Render
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, tex.ID);
    glBindVertexArray(m_VertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}





//The rest of these can be used for UI or Engine related handling

bool AppLayer::OnMouseButtonPressed(Core::MouseButtonPressedEvent& event)
{

    //Mouse button press handling.
    //Double check with TheCherno github to see how its used.
    if(m_InputManager->get_cursor() == false)
    {
        glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_InputManager->set_cursor(true);
    }

    return false;
}

bool AppLayer::OnMouseMoved(Core::MouseMovedEvent& event)
{
    //Mouse movement handling.
    m_MousePosition = {static_cast<float>(event.GetX()), static_cast<float>(event.GetY()) };
    //
    return false;
}


bool AppLayer::OnWindowClosed(Core::WindowClosedEvent& event)
{
    //Window closing Handling.
    std::println("Window Closed!");
    AppLayer::~AppLayer();
    return false;
}

bool AppLayer::OnKeyPressed(Core::KeyPressedEvent& event)
{
    //Keyboard Input handling ONLY for AppLayer

    //If ESC is pressed, we unlock the mouse from the camera, and ignore camera input.
    if(event.GetKeyCode() == GLFW_KEY_ESCAPE)
    {
        //Re-enable cursor, and signal the Input Manager that the cursor is unlocked.
        glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_InputManager->set_cursor(false);
    }
    return false;
}




