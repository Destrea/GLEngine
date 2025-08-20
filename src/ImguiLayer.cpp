
//ImGui layer for GLEngine


//#include "ImguiLayer.h"
//#include "FramebufferHandler.h"
#include "Common.h"
using namespace ImGui;

//FramebuffManager& i_FramebuffMgr = FramebuffManager::Get();
//ImGuiManager& i_ImguiMgr = ImGuiManager::Get();
ImGuiManager ImGuiManager::s_Instance;

void ImGuiManager::ImGuiRender(GLFWwindow* window)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ShowDockSpaceAndMenu();


    if(ImGui::Begin("Editor"))
    {
        bool isHovered = ImGui::IsWindowHovered();
        bool isFocused = ImGui::IsWindowFocused();

        if(isHovered)
        {
            if(ImGui::IsMouseDown(ImGuiMouseButton_Left))
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                InputManager::Get().set_cursor(true);
            }
        }
        if(isFocused)
        {

        }
        const float win_width = ImGui::GetContentRegionAvail().x;
        const float win_height = ImGui::GetContentRegionAvail().y;

        i_FramebuffMgr.rescale_framebuffer(win_width, win_height);
        glViewport(0,0, win_width, win_height);


        ImVec2 windowSize = ImGui::GetContentRegionAvail();
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 centralizedCursorpos = (windowSize - pos) * 0.5f;

        ImGui::SetCursorPos((GetContentRegionAvail() - ImVec2(i_FramebuffMgr.tex_w,i_FramebuffMgr.tex_h)) * 0.5f);
        ImVec2 size = ImVec2(i_FramebuffMgr.tex_w,i_FramebuffMgr.tex_h);
        ImVec2 uv0 = ImVec2(0,1);
        ImVec2 uv1 = ImVec2(1,0);

        //GLuint* texture = f_tex;
        ImGui::Image((GLuint*)(intptr_t)i_FramebuffMgr.f_tex, size, uv0, uv1);


    }
    ImGui::End();

    if(ImGui::Begin("New Window"))
    {
        ImGui::Text("This is a new Window");               // Display some text (you can use a format strings too)
        ImGui::InputInt("Editor Width", &i_FramebuffMgr.tex_w, 1, 100, ImGuiInputTextFlags_CharsDecimal);
        ImGui::InputInt("Editor Height", &i_FramebuffMgr.tex_h, 1, 100, ImGuiInputTextFlags_CharsDecimal);

    }
    ImGui::End();


    if(ImGui::Begin("New Window 2"))
    {
        ImGui::Text("This is ANOTHER new Window :)");

    }
    ImGui::End();

    if(ImGui::Begin("OpenGL Texture Text"))
    {
        ImGui::Text("pointer = %x", i_FramebuffMgr.f_tex);
        ImGui::Text("size = %d x %d", i_FramebuffMgr.tex_w, i_FramebuffMgr.tex_h);
        //ImGui::Image((ImTextureID)(intptr_t)f_tex, ImVec2(tex_w, tex_h));

    }
    ImGui::End();



     ImGui::EndFrame();
}


void ImGuiManager::ShowDockSpaceAndMenu()
{

    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {} // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

}


