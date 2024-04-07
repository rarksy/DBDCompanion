#include "Backend.hpp"
#include "Fonts/Rethink.hpp"
#include "../Features/Menu.h"
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include "Fonts/Inter-ExtraLight.hpp"

int backend::init_glfw()
{
    if (!glfwInit())
    {
        fprintf(stderr, "Error: Failed to initialize GLFW.\n");
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    return true;
}

void backend::shutdown_glfw()
{
    glfwDestroyWindow(menu::main_window);
    glfwTerminate();
}

GLFWwindow* backend::setup_window(const char* title, int width, int height)
{
    glfwWindowHint(GLFW_RESIZABLE, false);
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Error: Failed to create GLFW window.\n");
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    return window;
}

void backend::setup_imgui(GLFWwindow* window, ImGuiContext*& context)
{
    // Setup ImGui
    context = ImGui::CreateContext();
    ImGui::SetCurrentContext(context);
    auto& io = ImGui::GetIO();
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.Fonts->AddFontFromMemoryCompressedTTF(Rethink_compressed_data, Rethink_compressed_size, menu::styling::font_size);
    if (menu::styling::child_font == nullptr)
        menu::styling::child_font = io.Fonts->AddFontFromMemoryCompressedTTF(Rethink_compressed_data, Rethink_compressed_size, 15.F);
    io.IniFilename = nullptr;
}

bool backend::check_for_update()
{
    const std::string file_path = backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "version.json";
    nlohmann::json data;
    
    if (!ml::file_or_directory_exists(file_path) || ml::get_seconds_since_file_modified(file_path) > 1200)
    {
        data = ml::json_get_from_url("https://api.github.com/repos/rarksy/DBDCompanion/releases")[0];

        ml::json_write_data(file_path, data);
    }
    else
        data = ml::json_get_data_from_file(file_path);
    
    const std::string version = data["tag_name"];

    if (version != DBDC_VERSION)
    {
        updated_binary_url = data["assets"][0]["browser_download_url"];
        
        return true;
    }
}

bool backend::update()
{   
    if (updated_binary_url.empty())
        return false;

    ml::download_file(updated_binary_url, backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "new_binary.7z");


    return true;
    
}

void backend::shutdown_imgui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
