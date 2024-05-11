#include "shrine_of_secrets.hpp"
#include "backend/backend.hpp"
#include "../../misc/misc.hpp"
#include "GUI/GUI.h"
#include "images/images.h"
#include "ImGui/imgui.h"
#include "miscLIB/miscLIB.hpp"

bool shrine_of_secrets::is_cache_valid()
{
    const std::string cache_file = backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "shrine_cache.json";

    const int sec = ml::get_seconds_since_file_modified(cache_file);
    
    return sec < 3600 && sec != -1;
}

void shrine_of_secrets::cache()
{
    nlohmann::json data;

    for (int i = 0; i < 4; i++)
    {
        const auto perk = all_perks[i];
        data["shrine_data"]["perks"][i]["id"] = perk.id;
        data["shrine_data"]["perks"][i]["name"] = perk.name;
        data["shrine_data"]["perks"][i]["description"] = perk.description;
        data["shrine_data"]["perks"][i]["image_path"] = perk.image_path;
    }

    data["shrine_data"]["reset_time"] = reset_time_end;

    ml::json_write_data(backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "shrine_cache.json", data);
}

bool shrine_of_secrets::load_cache()
{
    nlohmann::json data = ml::json_get_data_from_file(backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "shrine_cache.json");

    if (data == nullptr)
        return false;

    shrine_data = data["shrine_data"];

    for (int i = 0; i < 4; i++)
    {
        perk p;
        p.id = shrine_data["perks"][i]["id"];
        p.name = shrine_data["perks"][i]["name"];
        p.description = ml::html_formatter(shrine_data["perks"][i]["description"]);
        p.image_path = shrine_data["perks"][i]["image_path"];

        all_perks.push_back(p);
    }

    reset_time_end = shrine_data["reset_time"];

    is_ready = true;
    return true;
}

void shrine_of_secrets::init()
{
    shrine_data = ml::json_get_from_url("https://dbd.tricky.lol/api/shrine?includeperkinfo=1");

    if (shrine_data.empty())
    {
        unavailable = true;
        return;
    }

    if (!shrine_data["error"].is_null())
        unavailable = true;
    else
    {
        for (int i = 0; i < 4; i++)
        {
            const auto perk_info = ml::json_get_from_url("https://dbd.tricky.lol/api/perkinfo?perk=" + shrine_data["perks"][i]["id"].get_ref<std::string&>());

            shrine_data["perks"][i]["image_path"] = std::string(misc::get_game_root_directory() + "DeadByDaylight/Content/" + std::string(perk_info["image"]));

            perk p;

            p.id = shrine_data["perks"][i]["id"];
            p.name = shrine_data["perks"][i]["name"];
            p.description = ml::html_formatter(shrine_data["perks"][i]["description"]);
            p.image_path = shrine_data["perks"][i]["image_path"];

            all_perks.push_back(p);
        }
        
        reset_time_end = shrine_data["end"];

        is_ready = true;
    }
}

bool shrine_of_secrets::load_images()
{
    bool success = true;
    for (int i = 0; i < 4; i++)
    {
        const auto perk = get_perk(i);

        GLuint texture;

        if (!std::filesystem::exists(perk.image_path))
            success = false;

        images::load_texture_from_file(get_perk(i).image_path, &texture);

        all_perk_images.push_back(texture);
    }

    return success;
}

shrine_of_secrets::perk shrine_of_secrets::get_perk(const int& perk_index)
{
    if (is_ready)
        return all_perks[perk_index];

    return {};
}

void shrine_of_secrets::render_ui()
{
    ImGui::SetCursorPos({10, 225});

    ImGui::BeginChild("ShrineChild", ImVec2(190, 0));

    ImGui::SeparatorText("Shrine Of Secrets");

    if (!is_ready && !unavailable)
    {
        const std::string loading_text = "Loading Data...";
        auto windowWidth = ImGui::GetWindowSize().x;
        auto textWidth = ImGui::CalcTextSize(loading_text.c_str()).x;

        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::TextWrapped(loading_text.c_str());
    }
    else if (!is_ready && unavailable)
    {
        const std::string loading_text = "Shrine Unavailable...";
        auto windowWidth = ImGui::GetWindowSize().x;
        auto textWidth = ImGui::CalcTextSize(loading_text.c_str()).x;

        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::TextWrapped(loading_text.c_str());
    }
    else
    {
        static bool has_loaded_images = false;
        if (!has_loaded_images)
        {
            can_use_images = load_images();
            has_loaded_images = true;
        }

        const std::string reset_text = "Resets In " + ml::unix_get_remaining_time(reset_time_end);
        auto windowWidth = ImGui::GetWindowSize().x;
        auto textWidth = ImGui::CalcTextSize(reset_text.c_str()).x;

        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::TextWrapped(reset_text.c_str());

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        for (int i = 0; i < 4; i++)
        {
            const auto perk_info = get_perk(i);
            const auto perk_name = perk_info.name;
            const auto perk_description = perk_info.description;

            const float image_size = 45.0f;

            if (can_use_images)
            {
                ImGui::Image(reinterpret_cast<void*>(all_perk_images[i]), ImVec2(image_size, image_size));
                ImGui::SameLine();
            }
            
            float available_width = ImGui::GetContentRegionAvail().x;
            
            float text_size = ImGui::CalcTextSize(perk_name.c_str()).x;
            if (text_size < available_width)
            {
                ImVec2 pos = ImGui::GetCursorPos();
                pos.y += 10;
                ImGui::SetCursorPos(pos);
            }

            ImGui::TextWrapped(perk_name.c_str());
            gui::tool_tip(perk_description, 500.f, false);
        }
    }

    ImGui::EndChild();
}
