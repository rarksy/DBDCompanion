#pragma once
#include <stdexcept>
#include <ImGui/imgui.h>

struct color
{
    int r;
    int g;
    int b;
    int a = 255;

    int& operator[](int index)
    {
        if (index < 0 || index > 3)
            throw std::out_of_range("Index Out Of Range, Use 0-3");

        switch (index)
        {
        case 0:
            return r;
        case 1:
            return g;
        case 2:
            return b;
        case 3:
            return a;
        default:
            break;
        }

        return a;
    }

    color() : r(0), g(0), b(0)
    {
    }

    color(int _r, int _g, int _b, int _a = 255) : r(_r), g(_g), b(_b), a(_a)
    {
    }

    ImVec4 to_imvec4() const
    {
        return {r / 255.F, g / 255.F, b / 255.F, a / 255.F};
    }

    ImColor to_imcolor() const
    {
        return ImColor(r, g, b, a);
    }

    void apply_from_imcolor(const ImColor& imcolor)
    {
        this->r = static_cast<int>(imcolor.Value.x);
        this->g = static_cast<int>(imcolor.Value.y);
        this->b = static_cast<int>(imcolor.Value.z);
        this->a = static_cast<int>(imcolor.Value.w);
    }

    void apply_from_imvec4(ImVec4 imvec4)
    {
        this->r = static_cast<int>(imvec4.x * 255.F);
        this->g = static_cast<int>(imvec4.y * 255.F);
        this->b = static_cast<int>(imvec4.z * 255.F);
        this->a = static_cast<int>(imvec4.w * 255.F);
    }

    ImColor* as_imcolor()
    {
        static ImColor im_color;
        im_color = this->to_imcolor();
        return &im_color;
    }

    ImVec4* as_imvec4()
    {
        static ImVec4 im_vec4;
        im_vec4 = this->to_imvec4();
        return &im_vec4;
    }
};