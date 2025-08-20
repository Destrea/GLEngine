#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "LevelLayout.h"
<<<<<<< HEAD
#include "Common.h"
=======
#include "include/Common.h"
>>>>>>> 4924f88 (Finished implementing Dear ImGui, reworked some systems into Singletons.)
#include "game.h"
#include "include/Camera.h"
#include "resource_manager.h"
#include <iostream>
#include "Model.h"
//#include "ImguiLayer.h"
<<<<<<< HEAD
#include <stb_image.h>


=======
#include "stb_image.h"

//Move some input callbacks into the InputManager.cpp file and call them with the pointer/singleton reference
>>>>>>> 4924f88 (Finished implementing Dear ImGui, reworked some systems into Singletons.)
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


GLFWwindow* window;

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

bool debug_mode;


Camera p_Camera(glm::vec3(0.0f,0.0f,3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


Game Default(SCR_WIDTH, SCR_HEIGHT);

int main(int argc, char *argv[])
{
    if(argc < 2 || argv[1] == "-d")
    {
        debug_mode = true;
    }
    else if(argv[1] == "-g")
    {
        debug_mode = false;
    }


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Default", nullptr, nullptr);
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;

    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //glfwSetKeyCallback(window, key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Glad: Load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);


    //Loads the shader for the level to be loaded.
    Shader ourShader = ResourceManager::LoadShader("src/Shaders/3.3.shader.vs", "src/Shaders/3.3.shader.fs", nullptr, "wall");

    //Loads the level with Level Loader
    level one;
    one.load_level("src/levels/Level1.txt");

    //Applies the shader to the loaded level mesh
    one.SetVars(ourShader);

    //setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
<<<<<<< HEAD
=======

    //Setup Dear ImGui io handler and settings/config loading/saving
>>>>>>> 4924f88 (Finished implementing Dear ImGui, reworked some systems into Singletons.)
    static ImGuiIO& io = ImGui::GetIO(); (void) io;
    ImGui::LoadIniSettingsFromDisk("imgui.ini");
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::SaveIniSettingsToDisk("imgui.ini");


    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }


    //Setup Platform/Renderer backends, with ImGui
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
<<<<<<< HEAD
    InputManager::Get().set_cursor(true);
=======

    //Set initial cursor lock state.
    InputManager::Get().set_cursor(true);

>>>>>>> 4924f88 (Finished implementing Dear ImGui, reworked some systems into Singletons.)

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);        //For rendering tris as wireframes


<<<<<<< HEAD
    Shader fb_shader = ResourceManager::LoadShader("src/Shaders/fb_shader.vs", "src/Shaders/fb_shader.fs", nullptr, "framebuff");


    //Framebuffer generation, for creating the "game" viewport via ImGui

    FramebuffManager::Get().init_framebuffer();




    //Framebuffer texture








=======
    //Loads the framebuffer shader. Mostly just places it in a certain spot, using the vertex shader, and applying the "framebuffer's" content
    Shader fb_shader = ResourceManager::LoadShader("src/Shaders/fb_shader.vs", "src/Shaders/fb_shader.fs", nullptr, "framebuff");
>>>>>>> 4924f88 (Finished implementing Dear ImGui, reworked some systems into Singletons.)


    //Framebuffer generation, for creating the "game" viewport via ImGui
    FramebuffManager::Get().init_framebuffer();

    //Initialize game
    Default.Init(window);

<<<<<<< HEAD




=======
>>>>>>> 4924f88 (Finished implementing Dear ImGui, reworked some systems into Singletons.)
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

<<<<<<< HEAD

        Default.calcDeltaTime();


        // Start the Dear ImGui frame


        ImGuiManager::Get().ImGuiRender(window);
        //ImGuiManager::Get().ShowDockSpaceAndMenu();

        ImGui::Render();

        //Split me into Game Input and Engine Input :)
        Default.ProcessInput(window, &p_Camera);


        GLuint fbo = FramebuffManager::Get().get_fbo();
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        one.Draw(ourShader,p_Camera);
=======
        //DeltaTime calculations, handled by Game.cpp
        Default.calcDeltaTime();


        //Renders the ImGui menus, handled within the ImGui layer
        ImGuiManager::Get().ImGuiRender(window);

        ImGui::Render();

        //Manage and process user input.
        Default.ProcessInput(window, &p_Camera);

>>>>>>> 4924f88 (Finished implementing Dear ImGui, reworked some systems into Singletons.)

        //Processes framebuffer for editor viewport
        GLuint fbo = FramebuffManager::Get().get_fbo();

<<<<<<< HEAD

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //manage user input
        //Breakout.ProcessInput(deltaTime);




        // update Game State
        //Default.Update(deltaTime);

        //Rendering
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        //Breakout.Render();
=======
        //Bind the fbo
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        //Add the editor background color, and clear the color and depth buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
>>>>>>> 4924f88 (Finished implementing Dear ImGui, reworked some systems into Singletons.)

        //Draw the level/model
        one.Draw(ourShader,p_Camera);

<<<<<<< HEAD



        //ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplGlfw_NewFrame();

        //


        //unsigned int* fb_tex = &f_tex;
        //


=======
        //unbind the fbo
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //Rendering for the "engine's" background color
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
>>>>>>> 4924f88 (Finished implementing Dear ImGui, reworked some systems into Singletons.)




<<<<<<< HEAD
        //Default.Render();

        //









=======
        //
>>>>>>> 4924f88 (Finished implementing Dear ImGui, reworked some systems into Singletons.)
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if(io.ConfigFlags && ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow * backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);

    }

<<<<<<< HEAD
=======
    //Framebuffer unbinding + shutdown
>>>>>>> 4924f88 (Finished implementing Dear ImGui, reworked some systems into Singletons.)
    FramebuffManager::Get().fbo_shutdown();


    //ImGui shutdown processes
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    //Delete all resources as loaded using the resource manager
    ResourceManager::Clear();
    glfwDestroyWindow(window);
    glfwTerminate();



    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //when user presses esc we set the glfwWindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Default.Keys[key] = true;
        else if (action == GLFW_RELEASE)
            Default.Keys[key] = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //Ensure that viewport matches the new, updated, window dimensions.
    glViewport(0,0,width, height);
}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);


    if(InputManager::Get().get_cursor() == true)
    {

        if(firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        p_Camera.ProcessMouseMovement(xoffset,yoffset);
    }
    else
    {
        //If the mouse isnt locked, we update the x and y pos so that the p_Camera doesnt whip when the mouse gets re-locked.
        lastX = xpos;
        lastY = ypos;

    }

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGuiIO& io = ImGui::GetIO();
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && InputManager::Get().get_cursor() == false)
    {
        if(io.WantCaptureMouse == false)
        {

        }
    }
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(InputManager::Get().get_cursor() == true)
        p_Camera.ProcessMouseScroll(static_cast<float>(yoffset));
}




GLFWwindow *get_Window()
{
    return window;
}

