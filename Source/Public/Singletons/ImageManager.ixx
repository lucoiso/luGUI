// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include "luGUIModule.hpp"

export module luGUI.UserInterface.Singletons.ImageManager;

export import RenderCore.Types.Texture;

namespace luGUI
{
    export struct LUGUIMODULE_API ImageData
    {
        VkDescriptorSet                      DescriptorSet { VK_NULL_HANDLE };
        std::shared_ptr<RenderCore::Texture> Texture { nullptr };
    };

    export class LUGUIMODULE_API ImageManager
    {
        std::hash<strzilla::string_view>           m_HashCreator {};
        std::unordered_map<std::size_t, ImageData> m_Textures {};

        ImageManager() = default;

        inline ~ImageManager()
        {
            UnregisterAllTextures();
        }

    public:
        [[nodiscard]] static ImageManager &Get();

        [[nodiscard]] inline ImageData GetImage(strzilla::string_view const &Key) const
        {
            if (std::empty(Key))
            {
                return ImageData {};
            }

            auto const Hash = m_HashCreator(Key);
            return m_Textures.contains(Hash) ? m_Textures.at(Hash) : ImageData {};
        }

        [[nodiscard]] inline VkDescriptorSet GetDescriptor(strzilla::string_view const &Key) const
        {
            if (std::empty(Key))
            {
                return VK_NULL_HANDLE;
            }

            auto const Hash = m_HashCreator(Key);
            return m_Textures.contains(Hash) ? m_Textures.at(Hash).DescriptorSet : VK_NULL_HANDLE;
        }

        [[nodiscard]] bool RegisterTexture(strzilla::string_view const &Key, std::shared_ptr<RenderCore::Texture> &&Texture);
        [[nodiscard]] bool RegisterTexture(strzilla::string_view const &Key, strzilla::string_view const &Path);
        void               RegisterTextureArray(std::unordered_map<strzilla::string_view, strzilla::string_view> &&KeysAndPaths);

        void UnregisterTexture(strzilla::string_view const &Key);
        void UnregisterAllTextures();

        void RefreshTextures();
    };
} // namespace luGUI
