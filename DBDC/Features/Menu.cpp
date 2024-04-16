#include "Menu.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <chrono>
#include <thread>
#include "ConfigEditor/CEMenu.hpp"
#include "Crosshair/CMenu.h"
#include "Crosshair/Crosshair.h"
#include "GUI/GUI.h"
#include <Windows.h>

#include "../Misc/Misc.hpp"
#include "HookTracker\HookTracker.hpp"
#include "OnScreenTimers/OnScreenTimers.hpp"
#include "IconPackager\IconPackager.hpp"
#include "IconPackager\IPMenu.hpp"
#include "Shrine Of Secrets/ShrineOfSecrets.hpp"

void menu::run_loop()
{
    {
        const auto accent_data = ml::json_get_data_from_file(backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "settings.json");

        if (accent_data.contains("menu_accent"))
        {
            styling::menu_accent.r = accent_data["menu_accent"]["r"];
            styling::menu_accent.g = accent_data["menu_accent"]["g"];
            styling::menu_accent.b = accent_data["menu_accent"]["b"];
            styling::menu_accent.a = accent_data["menu_accent"]["a"];
        }
    }

    std::thread shrine_load_thread([]
    {
        if (shrine_of_secrets::is_cache_valid())
            shrine_of_secrets::load_cache();
        else
        {
            shrine_of_secrets::init();

            if (!shrine_of_secrets::shrine_data.empty())
                shrine_of_secrets::cache();
        }
    });
    shrine_load_thread.detach();

    std::thread perk_packager_load_thread(perk_packager::setup);
    perk_packager_load_thread.detach();

    while (!glfwWindowShouldClose(main_window))
    {
        const double start_time = glfwGetTime();

        glfwMakeContextCurrent(menu::main_window);

        ImGui::SetCurrentContext(menu::main_context);
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        menu::create_global_style();
        menu::render_ui();

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(menu::main_window);

        if (menu::overlay::window != nullptr)
        {
            glfwMakeContextCurrent(menu::overlay::window);
            ImGui::SetCurrentContext(menu::overlay::context);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (hook_tracker::ht_vars::enabled)
                hook_tracker::render();

            if (cvars.enabled)
                Crosshair::DrawCrosshairs();

            if (onscreen_timers::enabled)
                onscreen_timers::render_timers();


            ImGui::Render();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(menu::overlay::window);
        }

        if (onscreen_timers::enabled)
            onscreen_timers::keypress_loop();

        const double end_time = glfwGetTime();
        const double elapsed_time = end_time - start_time;

        constexpr double target_frame_time = 1.0 / static_cast<double>(80);

        if (elapsed_time < target_frame_time)
        {
            const double sleep_time = target_frame_time - elapsed_time;
            std::this_thread::sleep_for(std::chrono::duration<double>(sleep_time));
        }
    }
}

void menu::render_ui()
{
    auto& style = ImGui::GetStyle();
    static bool hamburger_open = true;
    static float hamburger_width = 1.F;
    static float hamburger_height = 240.F;
    static float disabled_alpha = 0.01F;

    if (hamburger_open)
    {
        if (disabled_alpha > 0.1F)
        {
            disabled_alpha -= 0.04F;
            style.DisabledAlpha = disabled_alpha;
        }
        else style.DisabledAlpha = 0.1F;
    }
    else
    {
        if (disabled_alpha < 1.0F && hamburger_width > 0)
        {
            disabled_alpha += 0.04F;
            style.DisabledAlpha = disabled_alpha;
        }
        // figure out widget fade on menu close
    }

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(styling::menu_width, styling::menu_height), ImGuiCond_Once);
    ImGui::Begin("menu", nullptr, menu_flags);


    ImGui::BeginDisabled(hamburger_open || hamburger_width > 0);

    if (menu_to_show == 0)
    {
        shrine_of_secrets::render_ui();
    }

    if (menu_to_show == 1)
    {
        static bool setup = false;
        if (!setup)
        {
            setup = config_editor::initialize_config();
        }
        else
        {
            static std::once_flag setup_flag;
            std::call_once(setup_flag, CEMenu::Setup);
        }

        CEMenu::RenderUI();
    }

    else if (menu_to_show == 2)
    {
        static std::once_flag flag_menu;
        std::call_once(flag_menu, ip_menu::setup);

        ip_menu::render_ui();
    }

    else if (menu_to_show == 3)
    {
        static std::once_flag flag_crosshair;
        static std::once_flag flag_menu;
        std::call_once(flag_crosshair, Crosshair::Setup);
        std::call_once(flag_menu, CMenu::Setup);

        CMenu::RenderUI();
    }

    else if (menu_to_show == 4)
    {
        static std::once_flag flag_load;
        std::call_once(flag_load, onscreen_timers::load_timer_profile);

        onscreen_timers::render_ui();
    }

    ImGui::EndDisabled();

    ImGui::SetCursorPos(ImVec2(5, 5));

    const ImRect hamburger_activation_rect = ImRect({3, 3}, {39, 36});
    const auto hamburger_accent = ImGui::GetColorU32(ImGui::IsMouseHoveringRect(hamburger_activation_rect.Min, hamburger_activation_rect.Max)
                                                         ? ImGui::IsKeyDown(ImGuiKey_MouseLeft)
                                                               ? ImGuiCol_ButtonActive
                                                               : ImGuiCol_ButtonHovered
                                                         : ImGuiCol_Button
    );


    if (hamburger_width > 0.F)
    {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.07f, 0.07f, 0.07f, 1.F));
        gui::begin_group_box("hamburger", ImVec2(hamburger_width, hamburger_height));

        ImGui::GetWindowDrawList()->AddRectFilledMultiColor(
            {5, 5},
            {hamburger_width, hamburger_height},
            ImColor(std::clamp(styling::menu_accent.r - 200, 0, 255), 15, 15),
            ImColor(15, 13, 13),
            ImColor(15, 13, 13),
            ImColor(15, 13, 13)
        );

        ImGui::GetWindowDrawList()->AddRectFilled({11, 13}, {41, 18}, hamburger_accent, 4.F);
        ImGui::GetWindowDrawList()->AddRectFilled({11, 23}, {41, 28}, hamburger_accent, 4.F);
        ImGui::GetWindowDrawList()->AddRectFilled({11, 33}, {41, 38}, hamburger_accent, 4.F);

        ImGui::SetCursorPos({3, 3});
        if (ImGui::InvisibleButton("##HamburgerToggleButtonInsideMenu", {39, 36}))
            hamburger_open = !hamburger_open;

        static bool settings_open = false;
        const GLuint& settings_button_texture = (settings_open || menu_to_show != 0) ? icons::back_icon : icons::settings_icon;

        ImGui::SetCursorPos({173, 7});
        if (gui::image_button("settingsbutton", settings_button_texture, {23, 23}))
        {
            if (menu_to_show != 0)
                menu_to_show = 0;
            else
                settings_open = !settings_open;
        }

        if (!settings_open)
        {
            ImGui::SetCursorPosY(50.F);

            ImGui::PushFont(styling::child_font);
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Customization").x) * 0.5F);
            ImGui::TextColored(color(120, 120, 120, 185).to_imvec4(), "Customization");
            ImGui::PopFont();

            if (ImGui::Button("Config Editor", ImVec2(185, 0)))
                menu_to_show = 1;
            gui::tool_tip("Allows you to adjust your game settings in\nmore detail than the base game offers");

            ImGui::Spacing();

            if (ImGui::Button("Icon Packager", ImVec2(185, 0)))
                menu_to_show = 2;
            gui::tool_tip("");

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();

            ImGui::PushFont(styling::child_font);
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Overlay Features").x) * 0.5F);
            ImGui::TextColored(color(120, 120, 120, 185).to_imvec4(), "Overlay Features");
            ImGui::PopFont();

            if (ImGui::Button("Crosshair Overlay", ImVec2(185, 0)))
                menu_to_show = 3;
            gui::tool_tip("Allows you to use a crosshair overlay with many customization options");

            ImGui::Spacing();

            if (ImGui::Button("On-Screen Timers", ImVec2(185, 0)))
                menu_to_show = 4;
            gui::tool_tip("Allows you to setup hotkeys to display timers on your screen for relevant information");
        }
        else
        {
            ImGui::Spacing();
            gui::color_picker("Menu Accent", &menu::styling::menu_accent, true);
            if (ImGui::Checkbox("Launch With DBD", &ce_vars.launch_with_dbd))
            {
                if (ce_vars.launch_with_dbd)
                {
                    char file_name_buffer[MAX_PATH];
                    GetModuleFileNameA(NULL, file_name_buffer, MAX_PATH);

                    std::ofstream file_to_write(backend::exe_directory.string() + backend::settings_directory + "dual_load.bat");

                    if (file_to_write.is_open())
                    {
                        file_to_write
                            << "@echo off\n"
                            << "start \"\" \"" << misc::get_game_root_directory() << "DeadByDaylight.exe\" -provider Steam\n"
                            << "start \"\" \"" << file_name_buffer << "\"\n"
                            << "exit";

                        file_to_write.close();

                        ml::set_clipboard_text("\"" + backend::exe_directory.string() + backend::settings_directory + "dual_load.bat\" %command%");

                        MessageBox(
                            NULL,
                            L"Launch Command Copied\n\nGo To Steam -> Library -> Right Click \"Dead By Daylight\" -> Properties -> In The \"Launch Options\" Box -> Right Click -> Paste",
                            L"Notice", MB_OK);
                    }
                }
                else
                {
                    MessageBox(NULL, L"To Disable Dual Loading, Go To Steam -> Library -> Right Click \"Dead By Daylight\" -> Properties -> Clear The \"Launch Options\" Box",
                               L"Notice",
                               MB_OK);
                }
            }
            gui::tool_tip(
                "Will automatically open DBDC when you launch Dead By Daylight"
                "\n\n"
                "Note: Due to how steam works, after enabling, you will need to manually add the launch option that gets copied to your clipboard, instructions appear when enabling / disabling."
            );
            
            ImGui::SetCursorPos({5, 210});
            if (gui::image_button("discord_join_button", icons::discord_icon, ImVec2(31, 23)))
                ShellExecuteA(NULL, "open", "https://discord.gg/vKjjS8yazu", NULL, NULL, SW_SHOWNORMAL);
        }

        gui::end_group_box();

        static int previous_tab = menu_to_show;
        if (previous_tab != menu_to_show && !ImGui::IsMouseHoveringRect({0, 0}, {hamburger_width + 5, hamburger_height + 5}))
        {
            hamburger_open = false;
            previous_tab = menu_to_show;
            settings_open = false;
        }

        if (!ImGui::IsMouseHoveringRect({0, 0}, {hamburger_width + 5, hamburger_height + 5}) && !ImGui::IsAnyItemActive() && ImGui::IsKeyPressed(ImGuiKey_MouseLeft))
        {
            hamburger_open = false;
            settings_open = false;
        }
        ImGui::PopStyleColor();
    }

    if (hamburger_open && hamburger_width < 200)
        hamburger_width += 10;

    else if (!hamburger_open && hamburger_width > 0)
        hamburger_width -= 10;

    ImGui::GetWindowDrawList()->AddRectFilled({11, 13}, {41, 18}, hamburger_accent, 4.F);
    ImGui::GetWindowDrawList()->AddRectFilled({11, 23}, {41, 28}, hamburger_accent, 4.F);
    ImGui::GetWindowDrawList()->AddRectFilled({11, 33}, {41, 38}, hamburger_accent, 4.F);

    ImGui::SetCursorPos({7, 7});
    if (ImGui::InvisibleButton("##HamburgerToggleButtonOutsideMenu", {39, 36}))
        hamburger_open = !hamburger_open;

    ImGui::SetCursorPos({menu_to_show == 2 ? 10.F : 720.F, 470});
    ImGui::TextColored(ImVec4(0.8F, 0.8F, 0.8F, 0.5F), "(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Hold right click on any option\nto view information about it.");
        ImGui::Spacing();
        ImGui::Text("Press enter to join the discord.");
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::PushFont(styling::child_font);
        ImGui::Text("DBD Companion ( %s )", DBDC_VERSION.substr(7).c_str());
        ImGui::PopFont();

        ImGui::EndTooltip();

        if (ImGui::IsKeyPressed(ImGuiKey_Enter, false))
            ShellExecuteA(NULL, "open", "https://discord.gg/vKjjS8yazu", NULL, NULL, SW_SHOWNORMAL);
    }


    if (backend::update_available)
    {
        const ImVec2 cursor_pos = {menu_to_show == 2 ? 20.F : 695.F, 470};
        ImGui::SetCursorPos(cursor_pos);
        if (gui::image_button("updatebutton", icons::update_icon, ImVec2(23, 23)))
        {
            const auto result = MessageBoxA(NULL, "Download Latest Update?", "Notice", MB_YESNO);

            if (result == IDYES)
                backend::update();
        }
        gui::tool_tip("Update Available\n  Click To Install", 500, false);
    }

    ImGui::SetMouseCursor(ImGui::IsAnyItemHovered() ? ImGuiMouseCursor_Hand : ImGuiMouseCursor_Arrow);

    ImGui::End();
}

inline ImVec4 RGBToImVec4(int r, int g, int b, int a = 255)
{
    return {r / 255.F, g / 255.F, b / 255.F, a / 255.F};
}


void menu::create_global_style()
{
    ImGuiStyle& style = ImGui::GetStyle();
    auto& colors = style.Colors;

    // Button
    colors[ImGuiCol_Button] = styling::menu_accent.to_imvec4();
    colors[ImGuiCol_ButtonHovered] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 70, styling::menu_accent.b + 70);
    colors[ImGuiCol_ButtonActive] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 120, styling::menu_accent.b + 120);

    // Main Window
    colors[ImGuiCol_FrameBg] = RGBToImVec4(20, 20, 20);
    colors[ImGuiCol_FrameBgHovered] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 70, styling::menu_accent.b + 70);
    colors[ImGuiCol_FrameBgActive] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 120, styling::menu_accent.b + 120);
    style.FrameRounding = 2.F;
    style.DisabledAlpha = 0.3F;
    style.FrameBorderSize = 1.7F;
    //style.DisabledAlpha = 0.1f;
    style.ChildRounding = 3.F;

    // Slider
    colors[ImGuiCol_Slider] = styling::menu_accent.to_imvec4();
    colors[ImGuiCol_SliderActive] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 120, styling::menu_accent.b + 120);
    colors[ImGuiCol_SliderHovered] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 70, styling::menu_accent.b + 70);
    colors[ImGuiCol_SliderGrab] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g, styling::menu_accent.b);
    style.GrabRounding = style.FrameRounding;

    // Checkbox
    colors[ImGuiCol_CheckMark] = styling::menu_accent.to_imvec4();
    colors[ImGuiCol_CheckMarkHovered] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 70, styling::menu_accent.b + 70);
    colors[ImGuiCol_CheckMarkActive] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 120, styling::menu_accent.b + 120);

    // Combo
    colors[ImGuiCol_Combo] = styling::menu_accent.to_imvec4();
    colors[ImGuiCol_ComboActive] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 120, styling::menu_accent.b + 120);
    colors[ImGuiCol_ComboHovered] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 70, styling::menu_accent.b + 70);

    // Header ( Selectables Etc )
    colors[ImGuiCol_Header] = styling::menu_accent.to_imvec4();
    colors[ImGuiCol_HeaderHovered] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 70, styling::menu_accent.b + 70);
    colors[ImGuiCol_HeaderActive] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 120, styling::menu_accent.b + 120);

    // Separator
    colors[ImGuiCol_Separator] = styling::menu_accent.to_imvec4();

    // Input Text
    colors[ImGuiCol_InputText] = styling::menu_accent.to_imvec4();

    colors[ImGuiCol_TextSelectedBg] = RGBToImVec4(styling::menu_accent.r - 30, styling::menu_accent.g - 20, styling::menu_accent.b - 20, styling::menu_accent.a - 105);
}
