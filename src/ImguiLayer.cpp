
//ImGui layer for GLEngine


//#include "ImguiLayer.h"
//#include "FramebufferHandler.h"
//#include "../include/Common.h"
//#include <GLFW/glfw3.h>
#include "../include/InputManager.h"
#include "../include/LevelEditor.h"
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
    ShowLogWindow();


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


    if(ImGui::Begin("New Window 2"))
    {
        ImGui::Text("This is ANOTHER new Window :)");

    }
    ImGui::End();

    if(ImGui::Begin("OpenGL Texture Text"))
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


struct AppLog
{
    ImGuiTextBuffer     Buff;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets;
    bool                AutoScroll;

    AppLog()
    {
        AutoScroll = true;
        Clear();
    }

    void Clear()
    {
        Buff.clear();
        LineOffsets.clear();
        LineOffsets.push_back(0);
    }

    void AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        int old_size = Buff.size();
        va_list args;
        va_start(args, fmt);
        Buff.appendfv(fmt,args);
        va_end(args);
        for(int new_size = Buff.size(); old_size < new_size; old_size++)
        {
            if(Buff[old_size] == '\n')
            {
                LineOffsets.push_back(old_size+1);
            }
        }
    }

    void Draw(const char* title, bool* p_open = NULL)
    {
        if(!ImGui::Begin(title, p_open)){
            ImGui::End();
            return;
        }

        //Menu
        if(ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-Scroll", &AutoScroll);
            ImGui::EndPopup();
        }

        //Main window
        if(ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        bool clear = ImGui::Button("Clear");
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy");
        ImGui::SameLine();
        Filter.Draw("Filter", -100.0f);

        ImGui::Separator();

        if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (clear)
                Clear();
            if (copy)
                ImGui::LogToClipboard();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            const char* buf = Buff.begin();
            const char* buf_end = Buff.end();
            if (Filter.IsActive())
            {
                // In this example we don't use the clipper when Filter is enabled.
                // This is because we don't have random access to the result of our filter.
                // A real application processing logs with ten of thousands of entries may want to store the result of
                // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
                for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
                {
                    const char* line_start = buf + LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                    if (Filter.PassFilter(line_start, line_end))
                        ImGui::TextUnformatted(line_start, line_end);
                }
            }
            else
            {
                ImGuiListClipper clipper;
                clipper.Begin(LineOffsets.Size);
                while (clipper.Step())
                {
                    for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                    {
                        const char* line_start = buf + LineOffsets[line_no];
                        const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                        ImGui::TextUnformatted(line_start, line_end);
                    }
                }
                clipper.End();
            }
            ImGui::PopStyleVar();
            if(AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
        }
        ImGui::EndChild();
        ImGui::End();


    }
};



void ImGuiManager::ShowLogWindow()
{
    static AppLog log;

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
            log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
                       ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
            counter++;
        }
    }
    ImGui::End();

    log.Draw("Log");
}

