// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include "luGUIModule.hpp"

export module luGUI.UserInterface.Singletons.FontManager;

namespace luGUI
{
    export class LUGUIMODULE_API FontManager
    {
        std::hash<strzilla::string_view> m_HashCreator {};
        std::unordered_map<std::size_t, ImFont*> m_Fonts {};

        FontManager() = default;

        inline ~FontManager()
        {
            UnregisterAllFonts();
        }

    public:
        [[nodiscard]] static FontManager &Get();

        [[nodiscard]] inline ImFont* GetFont(strzilla::string_view const& Key) const
        {
            if (std::empty(Key))
            {
                return nullptr;
            }

            auto const Hash = m_HashCreator(Key);
            return m_Fonts.contains(Hash) ? m_Fonts.at(Hash) : nullptr;
        }

        [[nodiscard]] bool RegisterFont(strzilla::string_view const &Key, strzilla::string_view const &Path, float Pixels = 18.F);
        void BuildFonts();

        void UnregisterFont(strzilla::string_view const &Key);
        void UnregisterAllFonts();
    };
} // namespace luGUI
