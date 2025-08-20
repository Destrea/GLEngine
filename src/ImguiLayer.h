#ifndef IMGUIMGR_H
#define IMGUIMGR_H

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "include/Common.h"

#include "FramebufferHandler.h"
class ImGuiManager
{
public:
    ImGuiManager(const ImGuiManager&) = delete;
    void ImGuiRender(GLFWwindow* window);
    void ShowDockSpaceAndMenu();
<<<<<<< HEAD

=======
    void ShowLogWindow();
>>>>>>> 4924f88 (Finished implementing Dear ImGui, reworked some systems into Singletons.)
    static ImGuiManager& Get()
    {
        return s_Instance;
    }


<<<<<<< HEAD
=======

>>>>>>> 4924f88 (Finished implementing Dear ImGui, reworked some systems into Singletons.)
private:
    ImGuiManager() { }

    static ImGuiManager s_Instance;

    FramebuffManager& i_FramebuffMgr = FramebuffManager::Get();

};

//ImGuiManager ImGuiManager::s_Instance;


#endif
