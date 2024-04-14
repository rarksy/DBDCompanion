#pragma once

#include "miscLIB/miscLIB.hpp"

namespace perk_packager
{
    bool setup();
    void clear_images();
    void reload();

    bool get_endpoint_data(const std::string& endpoint, nlohmann::json& data);
    template <typename T>
    void load_instance_data(std::vector<T>* vec_obj, const nlohmann::json& data);
    template <typename T>
    void clear_instance_images(std::vector<T>& vec_obj);
    template <typename T>
    void reload_package_data(std::vector<T>& vec_obj);

    struct base
    {
        std::string owner;

        std::string name;
        std::string id;
        std::string role;

        unsigned image = -1;
        std::string local_image_path;
        std::string game_file_path;

        mutable bool has_selected_image = false;
    };

    struct perk : base
    {
        std::string base_type = "Perk";
    };

    struct item : base
    {
        std::string base_type = "Item";
    };

    struct power : base
    {
        std::string base_type = "Power";
    };

    struct offering : base
    {
        std::string base_type = "Offering";
    };

    struct addon : base
    {
        std::string base_type = "Addon";
    };

    struct portrait : base
    {
        std::string base_type = "Portrait";
    };

    struct status_effect : base
    {
        std::string base_type = "Status Effect";
    };

    inline std::vector<perk> all_perks;
    inline std::vector<item> all_items;
    inline std::vector<power> all_powers;
    inline std::vector<offering> all_offerings;
    inline std::vector<addon> all_addons;
    inline std::vector<portrait> all_portraits;
    inline std::vector<status_effect> all_status_effects;

    namespace _internal
    {
        inline bool unavailable = false;

        inline nlohmann::json all_characters_data;
        inline nlohmann::json all_perks_data;
        inline nlohmann::json all_items_data;
        inline nlohmann::json all_powers_data;
        inline nlohmann::json all_offerings_data;
        inline nlohmann::json all_addons_data;

        inline nlohmann::json package_data;

        inline std::vector<std::string> all_survivor_general_perks =
        {
            "Dark_Sense",
            "Deja_Vu",
            "Hope",
            "Kindred",
            "Lightweight",
            "No_One_Left_Behind",
            "Plunderers_Instinct",
            "Premonition",
            "Resilience",
            "Slippery_Meat",
            "Small_Game",
            "Spine_Chill",
            "This_Is_Not_Happening",
            "WellMakeIt",
        };

        inline std::vector<std::string> all_killer_general_perks =
        {
            "Bitter_Murmur",
            "Deerstalker",
            "Distressing",
            "No_One_Escapes_Death",
            "Hex_Thrill_Of_The_Hunt",
            "Insidious",
            "Iron_Grasp",
            "Monstrous_Shrine",
            "BoonDestroyer",
            "Sloppy_Butcher",
            "Spies_From_The_Shadows",
            "Unrelenting",
            "Whispers",
        };

        inline std::vector<std::pair<std::string, std::string>> all_status_effects_data =
        {
            {"Bleeding", "bleeding"},
            {"Blessed", "blessed"},
            {"Bloodlust", "bloodlust"},
            {"Broken", "broken"},
            {"Cleansing", "cleansing"},
            {"Deep Wound", "deepWound"},
            {"Endurance", "endurance"},
            {"Exhausted", "exhausted"},
            {"Expertise", "expertise"},
            {"Exposed", "exposed"},
            {"Glyph", "glyph"},
            {"Healing", "healing"},
            {"Hearing", "hearing"},
            {"Hindered", "hindered"},
            {"Incapacitated", "incapacitated"},
            {"Killer Endurance", "killerEndurance"},
            {"Luck", "luck"},
            {"Madness", "madness"},
            {"Mangled", "mangled"},
            {"Mori", "mori"},
            {"Oblivious", "oblivious"},
            {"Progression Speed", "progressionSpeed"},
            {"Reparing", "repairing"},
            {"Sabotaging", "sabotaging"},
            {"Skill Check Difficulty", "skillCheckDifficulty"},
            {"Skill Check Probability", "skillCheckProbability"},
            {"Skills", "skills"},
            {"Sleep Penalty", "sleepPenalty"},
            {"Speed", "speed"},
            {"Undetectable", "undetectable"},
            {"Vision", "vision"},
        };

    }
}
