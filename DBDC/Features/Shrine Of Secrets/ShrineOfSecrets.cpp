#include "ShrineOfSecrets.hpp"

#include "../../Misc/Misc.hpp"
#include "../GUI/GUI.h"
#include "Images/Images.h"
#include "ImGui/imgui.h"
#include "miscLIB/miscLIB.hpp"

bool shrine_of_secrets::is_cache_valid()
{
    const std::string cache_file = backend::exe_directory.string() + "\\DBDC\\shrine_cache.json";

    if (!std::filesystem::exists(cache_file))
        return false;

    struct stat result;

    if (stat(cache_file.c_str(), &result) == 0)
    {
        time_t mod_time = result.st_mtime;
        time_t current_time = time(nullptr);
        double seconds_since_modification = difftime(current_time, mod_time);

        return (seconds_since_modification < 3600); // 1h
    }
    return false;
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

    std::ofstream file_to_write(backend::exe_directory.string() + "\\DBDC\\shrine_cache.json");
    if (file_to_write.is_open())
    {
        file_to_write << data.dump(4);
        file_to_write.close();
    }
}

bool shrine_of_secrets::load_cache()
{
    std::ifstream file_to_read(backend::exe_directory.string() + "\\DBDC\\shrine_cache.json");

    if (file_to_read.is_open())
    {
        nlohmann::json data;

        file_to_read >> data;

        shrine_data = data["shrine_data"];

        for (int i = 0; i < 4; i++)
        {
            perk p;
            p.id = shrine_data["perks"][i]["id"];
            p.name = shrine_data["perks"][i]["name"];
            p.description = shrine_data["perks"][i]["description"];
            p.image_path = shrine_data["perks"][i]["image_path"];

            all_perks.push_back(p);
        }

        reset_time_end = shrine_data["reset_time"];

        is_ready = true;
        return true;
    }

    return false;
}

void shrine_of_secrets::init()
{
    shrine_data = ml::json_get("https://dbd.tricky.lol/api/shrine?includeperkinfo=1");

    if (!shrine_data["error"].is_null())
        unavailable = true;
    else
    {
        for (int i = 0; i < 4; i++)
        {
            const auto perk_info = ml::json_get("https://dbd.tricky.lol/api/perkinfo?perk=" + shrine_data["perks"][i]["id"].get_ref<std::string&>());

            shrine_data["perks"][i]["image_path"] = std::string(misc::get_game_root_directory() + "DeadByDaylight/Content/" + std::string(perk_info["image"]));

            perk p;

            p.id = shrine_data["perks"][i]["id"];
            p.name = shrine_data["perks"][i]["name"];
            p.description = shrine_data["perks"][i]["description"];
            p.image_path = shrine_data["perks"][i]["image_path"];

            all_perks.push_back(p);
        }
    }

    reset_time_end = shrine_data["end"];

    is_ready = true;
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
    ImGui::SetCursorPos({10, 175});

    ImGui::BeginChild("ShrineChild", ImVec2(190, 0));

    ImGui::SeparatorText("Shrine Of Secrets");

    if (!is_ready)
    {
        const std::string loading_text = "Loading Data...";
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

        for (int i = 0; i < 4; i++)
        {
            const auto perk_info = get_perk(i);

            const auto perk_name = perk_info.name;
            const auto image_path = perk_info.image_path;

            const auto perk_description = ml::html_formatter(perk_info.description);


            const float image_size = 45.0f;
            const float text_height = ImGui::CalcTextSize(perk_name.c_str()).y;
            if (can_use_images)
            {
                ImGui::Image(reinterpret_cast<void*>(all_perk_images[i]), ImVec2(image_size, image_size));
                ImGui::SameLine();
            }
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (image_size - text_height) / 2);
            ImGui::Text(perk_name.c_str());
            gui::tool_tip(perk_description, 500.f, false);
        }
    }

    ImGui::EndChild();
}
