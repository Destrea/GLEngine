
//ImGui layer for GLEngine


#include "../include/ImguiLayer.h"
//#include "FramebufferHandler.h"
//#include "../include/Common.h"
//#include <GLFW/glfw3.h>
#include "../include/InputManager.h"
#include "../include/LevelEditor.h"
#include "../include/game.h"
using namespace ImGui;

//FramebuffManager& i_FramebuffMgr = FramebuffManager::Get();
//ImGuiManager& i_ImguiMgr = ImGuiManager::Get();
ImGuiManager ImGuiManager::s_Instance;
static ImGuiManager::AppLog logMessages;

void ImGuiManager::ImGuiRender(GLFWwindow* window)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ShowDockSpaceAndMenu();
    ShowLogWindow();



    //Refine this section to make them all Unique, so that it doesn't cause issues with the list in ImGui


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

        FramebuffManager::Get().rescale_framebuffer(win_width, win_height);
        glViewport(0,0, win_width, win_height);


        ImVec2 windowSize = ImGui::GetContentRegionAvail();
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 centralizedCursorpos = (windowSize - pos) * 0.5f;

        ImGui::SetCursorPos((GetContentRegionAvail() - ImVec2(FramebuffManager::Get().tex_w,FramebuffManager::Get().tex_h)) * 0.5f);
        ImVec2 size = ImVec2(FramebuffManager::Get().tex_w,FramebuffManager::Get().tex_h);
        ImVec2 uv0 = ImVec2(0,1);
        ImVec2 uv1 = ImVec2(1,0);

        //GLuint* texture = f_tex;
        ImGui::Image((GLuint*)(intptr_t)FramebuffManager::Get().f_tex, size, uv0, uv1);


    }
    ImGui::End();

    if(ImGui::Begin("New Window"))
    {
        ImGui::Text("This is a new Window");               // Display some text (you can use a format strings too)
        ImGui::InputInt("Editor Width", &FramebuffManager::Get().tex_w, 1, 100, ImGuiInputTextFlags_CharsDecimal);
        ImGui::InputInt("Editor Height", &FramebuffManager::Get().tex_h, 1, 100, ImGuiInputTextFlags_CharsDecimal);

    }
    ImGui::End();


    if(ImGui::Begin("Inspect"))
    {
        int inspectedObject = LevelEditor::Get().selectedObj;
        LevelEditor::EditorVertex* selectedVert;

        if(inspectedObject != 0)
        {
            selectedVert = LevelEditor::Get().getVertInfo(inspectedObject);

        }

        ImGui::Text("Object ID: %d", inspectedObject);
        ImGui::Text("Position");
        ImGui::InputFloat("X", &selectedVert->location.x); ImGui::SameLine();
        ImGui::InputFloat("Y", &selectedVert->location.y); //ImGui::SameLine();
        //ImGui::SliderFloat("Z", &f2, 0.0f, 5.0f);





    }
    ImGui::End();

    if(ImGui::Begin("Vertex Properties"))
    {
        ImGui::Text("pointer = %x", FramebuffManager::Get().f_tex);
        ImGui::Text("size = %d x %d", FramebuffManager::Get().tex_w, FramebuffManager::Get().tex_h);
        //ImGui::Image((ImTextureID)(intptr_t)f_tex, ImVec2(tex_w, tex_h));




    }
    ImGui::End();

    LevelEditor::Get().DrawWindow();

     ImGui::EndFrame();
}


void ImGuiManager::ShowDockSpaceAndMenu()
{
    bool exportPopup = false;

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



    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

}




void ImGuiManager::addLogItem(std::string logtype, std::string message)
{
    const char* charMsg = message.data();
    const char* charLogtype = logtype.data();
    logMessages.AddLog("[%05d] [%s] %s \n", ImGui::GetFrameCount(), charLogtype, charMsg);
}



void ImGuiManager::ShowLogWindow()
{

    ImGui::SetNextWindowSize(ImVec2(500,400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Log");
    if(ImGui::SmallButton("[Debug] Add 5 entries"))
    {
        static int counter = 0;
        const char* categories[3] = {"info", "warn", "error"};
        const char* words[] = {"problem", "situation", "issue", "information", "oof ouch owie"};
        for (int n = 0; n < 5; n++)
        {
            const char* category = categories[counter % IM_ARRAYSIZE(categories)];
            const char* word = words[counter % IM_ARRAYSIZE(words)];
            logMessages.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
                       ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
            counter++;
        }
    }
    ImGui::End();

    logMessages.Draw("Log");
}

