// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include "luGUIModule.hpp"

#include <imgui.h>

export module luGUI.UserInterface.Items.Image;

export import luGUI.UserInterface.Items.Item;

namespace luGUI
{
    export struct ImageDefinitions
    {
        ImVec2 Size { ImVec2 { 16.F, 16.F } };
        ImVec2 UV0 { ImVec2 { 0.F, 0.F } };
        ImVec2 UV1 { ImVec2 { 1.F, 1.F } };
        ImVec4 ImageColor { ImVec4 { 1.F, 1.F, 1.F, 1.F } };
        ImVec4 BorderColor { ImVec4 { 0.F, 0.F, 0.F, 0.F } };
    };

    export class LUGUIMODULE_API Image : public Item
    {
        strzilla::string m_Key {};
        ImageDefinitions m_Definitions {};

    public:
        Image() = default;
        explicit Image(float Width);
        explicit Image(strzilla::string_view const &Key, float Width = 0.F);
        explicit Image(strzilla::string_view const &Key, ImageDefinitions &&Definitions, float Width = 0.F);

        [[nodiscard]] inline strzilla::string_view GetKey() const
        {
            return m_Key;
        }

        inline void SetKey(strzilla::string_view const &Key)
        {
            m_Key = Key;
        }

        [[nodiscard]] inline ImageDefinitions const &GetDefinitions() const
        {
            return m_Definitions;
        }

        inline void SetDefinitions(ImageDefinitions &&Definitions)
        {
            m_Definitions = Definitions;
        }

    protected:
        void Render() override;
    };
} // namespace luGUI
