#pragma once

#include "miscLIB/miscLIB.hpp"

namespace PerkPackager
{
    void Setup();

    using json = nlohmann::json;
    inline json survivorData;
    inline json killerData;

    struct Character
    {
        std::string id;
        std::string name;
        std::string dlc = "null";

        Character(std::string _id) : id(_id)
        {
        }
    };

    namespace Characters
    {
        inline Character dwight("0");
        inline Character meg("1");
        inline Character claudette("2");
        inline Character jake("3");
        inline Character nea("4");
        inline Character laurie("5");
        inline Character ace("6");
        inline Character bill("7");
        inline Character feng("8");
        inline Character david("9");
        inline Character kate("10");
        inline Character quentin("11");
        inline Character tapp("12");
        inline Character adam("13");
        inline Character jeff("14");
        inline Character jane("15");
        inline Character ash("16");
        inline Character nancy("17");
        inline Character steve("18");
        inline Character yui("19");
        inline Character zarina("20");
        inline Character cheryl("21");
        inline Character felix("22");
        inline Character elodie("23");
        inline Character yunjinlee("24");
        inline Character jill("25");
        inline Character leon("26");
        inline Character mikaela("27");
        inline Character jonah("28");
        inline Character yoichi("29");
        inline Character haddie("30");
        inline Character ada("31");
        inline Character rebecca("32");
        inline Character vittorio("33");
        inline Character thalita("34");
        inline Character renato("35");
        inline Character gabriel("36");
        inline Character nicolas("37");
        inline Character ellen("38");
        inline Character alan("39");
        
        inline std::vector allSurvivors = {
            dwight, meg, claudette, jake, nea, laurie, bill, feng, david, kate, quentin, tapp, adam, jeff, jane, ash, nancy, steve, yui, zarina, cheryl,
            felix, elodie, yunjinlee, jill, leon, mikaela, jonah, yoichi, haddie, ada, rebecca, vittorio, thalita, renato, gabriel, nicolas, ellen, alan
        };
    }
}
