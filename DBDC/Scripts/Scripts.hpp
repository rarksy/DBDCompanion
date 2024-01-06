#pragma once
#include <string>

namespace Scripts
{
    namespace DisableTextureStreaming
    {
        inline std::string script =
            "import configparser\n"
            "import unreal\n"
            "\n"
            "[SystemSettings]\n"
            "r.TextureStreaming=0\n"
            "r.Streaming.PoolSize=0\n"
            "r.Streaming.HLODStrategy=2\n"
            "\n"
            "[TextureStreaming]\n"
            "AllowStreaming=False\n"
            "\n"
            "[TextureQuality@4]\n"
            "r.MaxAnisotropy=32\n"
            "r.Streaming.MipBias=0\n"
            "r.Streaming.Boost=-1\n"
            "r.Streaming.PoolSize=32768\n"
            "r.Streaming.LimitPoolSizeToVRAM=0\n"
            "\n"
            "[TextureQuality@3]\n"
            "r.MaxAnisotropy=32\n"
            "r.Streaming.MipBias=0\n"
            "r.Streaming.Boost=-1\n"
            "r.Streaming.PoolSize=32768\n"
            "r.Streaming.LimitPoolSizeToVRAM=0\n"
            "\n"
            "[TextureQuality@2]\n"
            "r.MaxAnisotropy=32\n"
            "r.Streaming.MipBias=0\n"
            "r.Streaming.Boost=-1\n"
            "r.Streaming.PoolSize=32768\n"
            "r.Streaming.LimitPoolSizeToVRAM=0\n"
            "\n"
            "[TextureQuality@1]\n"
            "r.MaxAnisotropy=32\n"
            "r.Streaming.MipBias=0\n"
            "r.Streaming.Boost=-1\n"
            "r.Streaming.PoolSize=32768\n"
            "r.Streaming.LimitPoolSizeToVRAM=0\n"
            "\n"
            "[TextureQuality@0]\n"
            "r.MaxAnisotropy=32\n"
            "r.Streaming.MipBias=0\n"
            "r.Streaming.Boost=-1\n"
            "r.Streaming.PoolSize=32768\n"
            "r.Streaming.LimitPoolSizeToVRAM=0\n"
            "\n"
            "[/Script/Engine.RendererSettings]\n"
            "r.TextureStreaming=0";
    }

    namespace DisableMouseAcceleration
    {
        inline std::string script =
            "import configparser\n"
            "import unreal\n"
            "\n"
            "[/Script/Engine.InputSettings]\n"
            "bEnableMouseSmoothing=False\n"
            "bDisableMouseAcceleration=True";
    }
}
