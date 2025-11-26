
#include "ImGuiLayer.h"
#include "InputManager.h"
#include <print>

ImGuiLayer::ImGuiLayer()
{
    BeginEngineUI();
    printf("Beginning ImGuiLayer!");
}

ImGuiLayer::~ImGuiLayer()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    printf("Killing ImGuiLayer!");
}


void ImGuiLayer::OnEvent(Core::Event& event)
{
    //Debugging console output, tied to every event.
    //std::println("{}", event.ToString());

    Core::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) {return OnMouseButtonPressed(e); });

    dispatcher.Dispatch<Core::MouseMovedEvent>([this](Core::MouseMovedEvent& e) {return OnMouseMoved(e);});

    dispatcher.Dispatch<Core::WindowClosedEvent>([this](Core::WindowClosedEvent& e) {return OnWindowClosed(e); });

    dispatcher.Dispatch<Core::KeyPressedEvent>([this](Core::KeyPressedEvent& e) {return OnKeyPressed(e); });
}



void ImGuiLayer::toggleHidden()
{
    m_IsHidden = !m_IsHidden;
}

void ImGuiLayer::OnUpdate(float ts)
{

    if(!m_IsHidden)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("File"))
            {
                if(ImGui::BeginMenu("LevelEditor"))
                {
                    if(ImGui::MenuItem("Export")){}


                    //ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                    //ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));



                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui::EndFrame();
        ImGui::UpdatePlatformWindows();
    }


}




void ImGuiLayer::OnRender()
{
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(!m_IsHidden)
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if(m_io.ConfigFlags && ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow * backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
}

void ImGuiLayer::BeginEngineUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    static ImGuiIO& io = ImGui::GetIO(); (void) io;
    m_io = io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    window = Core::Application::Get().GetWindow()->GetHandle();
    //ImGui window handling
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}




bool ImGuiLayer::OnMouseButtonPressed(Core::MouseButtonPressedEvent& event)
{

    //Mouse button press handling.
    //Double check with TheCherno github to see how its used.
    return false;
}

bool ImGuiLayer::OnMouseMoved(Core::MouseMovedEvent& event)
{
    //Mouse movement handling.

    return false;
}


bool ImGuiLayer::OnWindowClosed(Core::WindowClosedEvent& event)
{
    //Window closing Handling.
    std::println("Window Closed!");
    ImGuiLayer::~ImGuiLayer();
    return false;
}

bool ImGuiLayer::OnKeyPressed(Core::KeyPressedEvent& event)
{
    //Keyboard Input handling ONLY for AppLayer

    if(event.GetKeyCode() == GLFW_KEY_J)
    {
        toggleHidden();
    }

    return false;
}



