#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "LevelLayout.h"
#include "Common.h"
#include "game.h"
#include "include/Camera.h"
#include "resource_manager.h"
#include <iostream>
#include "Model.h"
//#include "ImguiLayer.h"
#include <stb_image.h>


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

    //ResourceManager::LoadShader("src/Shaders/1.model_loading.vs", "src/Shaders/1.model_loading.fs", nullptr, "backpack");

    //Shader ourShader = ResourceManager::GetShader("backpack");

    //Model ourModel("src/backpack/backpack.obj");

    Shader ourShader = ResourceManager::LoadShader("src/Shaders/3.3.shader.vs", "src/Shaders/3.3.shader.fs", nullptr, "wall");


    level one;
    one.load_level("src/levels/Level1.txt");

    one.SetVars(ourShader);

    //setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
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


    //Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    InputManager::Get().set_cursor(true);


    //Model ourModel = ResourceManager::StoreModel("src/backpack/backpack.obj", "backpack");

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //OpenGL configuration
    //glViewport(0,0, SCR_WIDTH, SCR_HEIGHT);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    Shader fb_shader = ResourceManager::LoadShader("src/Shaders/fb_shader.vs", "src/Shaders/fb_shader.fs", nullptr, "framebuff");


    //Framebuffer generation, for creating the "game" viewport via ImGui

    FramebuffManager::Get().init_framebuffer();




    //Framebuffer texture










    //Initialize game
    Default.Init(window);





    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();


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



        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //manage user input
        //Breakout.ProcessInput(deltaTime);




        // update Game State
        //Default.Update(deltaTime);

        //Rendering
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        //Breakout.Render();

        //model = glm::scale(model, glm::vec3(1.0f,1.0f,1.0f));




        //ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplGlfw_NewFrame();

        //


        //unsigned int* fb_tex = &f_tex;
        //






        //Default.Render();

        //









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

