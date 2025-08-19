#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "LevelLayout.h"
#include "game.h"
#include "include/Camera.h"
#include "resource_manager.h"
#include <iostream>
#include "Model.h"
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

void set_cursor(bool val);
bool get_cursor();

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

Camera camera(glm::vec3(0.0f,0.0f,3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool cursor_locked;

//DeltaTime variables
float deltaTime = 0.0f;
float lastFrame = 0.0f;


//ImGui stuff

Game Default(SCR_WIDTH, SCR_HEIGHT);

int main(int argc, char *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Default", nullptr, nullptr);
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
    cursor_locked = true;

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
    ImGuiIO& io = ImGui::GetIO();
    ImGui::LoadIniSettingsFromDisk("ImGuiConfig.ini");
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::SaveIniSettingsToDisk("ImGuiConfig.ini");




    //Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();



    //Model ourModel = ResourceManager::StoreModel("src/backpack/backpack.obj", "backpack");

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //OpenGL configuration
    //glViewport(0,0, SCR_WIDTH, SCR_HEIGHT);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);




    //Initialize game
    Default.Init(window);



    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(); // Show demo window! :)


        //Calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;





        //manage user input
        //Breakout.ProcessInput(deltaTime);


        if(!io.WantCaptureMouse)
        {
            processInput(window);
        }

        // update Game State
        //Default.Update(deltaTime);

        //Rendering

        //glClearColor(0.992156f,0.2392156f,0.7098039f, 1.0f);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //Breakout.Render();

        //model = glm::scale(model, glm::vec3(1.0f,1.0f,1.0f));

        one.Draw(ourShader,camera);




        //Default.Render();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            // TODO for OpenGL: restore current GL context.
        }

        glfwSwapBuffers(window);



    }


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


    if(get_cursor() == true)
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

        camera.ProcessMouseMovement(xoffset,yoffset);
    }
    else
    {
        //If the mouse isnt locked, we update the x and y pos so that the camera doesnt whip when the mouse gets re-locked.
        lastX = xpos;
        lastY = ypos;

    }

}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

    ImGuiIO& io = ImGui::GetIO();
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && get_cursor() == false)
    {
        std::cout << io.WantCaptureMouse << std::endl;
        if(io.WantCaptureMouse == false)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            set_cursor(true);
        }
    }



}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(get_cursor() == true)
        camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && get_cursor() == true)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        set_cursor(false);
    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}



void set_cursor(bool val)
{
    cursor_locked = val;
}

bool get_cursor()
{
    return cursor_locked;
}

