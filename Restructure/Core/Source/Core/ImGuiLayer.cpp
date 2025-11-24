
#include "ImGuiLayer.h"
#include "InputManager.h"

ImGuiLayer::ImGuiLayer()
{
    BeginEngineUI();
}

ImGuiLayer::~ImGuiLayer()
{
    EndEngineUI();
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


void ImGuiLayer::EndEngineUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
