#ifndef IMGUIMGR_H
#define IMGUIMGR_H

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../extern/ImGui/imgui.h"
#include "../extern/ImGui/imgui_impl_glfw.h"
#include "../extern/ImGui/imgui_impl_opengl3.h"
#include "../extern/ImGui/imgui_internal.h"

#include "../include/Common.h"

#include "FramebufferHandler.h"
class ImGuiManager
{
public:
    ImGuiManager(const ImGuiManager&) = delete;
    void ImGuiRender(GLFWwindow* window);
    void ShowDockSpaceAndMenu();
    void ShowLogWindow();
    void addLogItem(std::string logtype, std::string message);
    static ImGuiManager& Get()
    {
        return s_Instance;
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


private:
    ImGuiManager() { }

    static ImGuiManager s_Instance;



    //FramebuffManager& i_FramebuffMgr = FramebuffManager::Get();

};

//ImGuiManager ImGuiManager::s_Instance;


#endif
