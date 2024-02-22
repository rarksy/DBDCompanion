#include "PerkPackager.h"

#include "PPMenu.h"

void PerkPackager::Setup()
{
    survivorData = ml::json_get("https://dbd.tricky.lol/api/characters?role=survivor&includeperks=1");

    for (auto& survivor : Characters::allSurvivors)
    {
        survivor.name = survivorData[survivor.id]["name"];

        if (!survivorData[survivor.id]["dlc"].is_null())
            survivor.dlc = survivorData[survivor.id]["dlc"];

        std::cout << survivor.dlc << std::endl;
    }


    PPMenu::isSetup = true;
}
