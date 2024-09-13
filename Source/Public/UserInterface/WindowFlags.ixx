// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

export module luGUI.UserInterface.Window.Flags;

import RenderCore.Utils.EnumHelpers;

namespace luGUI
{
    export enum class InitializationFlags : std::uint8_t {
        NONE             = 0,
        MAXIMIZED        = 1 << 0,
        HEADLESS         = 1 << 1,
        ENABLE_VIEWPORTS = 1 << 2,
        ENABLE_DOCKING   = 1 << 3,
        WITHOUT_TITLEBAR = 1 << 4,
        ALWAYS_ON_TOP    = 1 << 5,
    };
} // namespace luGUI
