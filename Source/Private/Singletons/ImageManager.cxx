// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

module luGUI.UserInterface.Singletons.ImageManager;

import luGUI.Integrations.ImGuiVulkanBackend;
import RenderCore.Renderer;
import RenderCore.Runtime.Scene;
import RenderCore.Factories.Texture;
import RenderCore.Utils.Constants;

using namespace luGUI;

ImageManager &ImageManager::Get()
{
    static ImageManager Singleton {};
    return Singleton;
}

bool ImageManager::RegisterTexture(strzilla::string_view const &Key, std::shared_ptr<RenderCore::Texture> &&Texture)
{
    if (std::empty(Key) || Texture == nullptr)
    {
        return false;
    }

    auto const Hash = m_HashCreator(Key);
    if (m_Textures.contains(Hash))
    {
        return false;
    }

    VkSampler const Sampler { RenderCore::GetSampler() };
    if (Sampler == VK_NULL_HANDLE)
    {
        return false;
    }

    VkImageView const View = Texture->GetImageDescriptor().imageView;
    if (View == VK_NULL_HANDLE)
    {
        return false;
    }

    m_Textures.emplace(Hash, ImageData { .DescriptorSet = ImGuiVulkanAddTexture(Sampler, View, RenderCore::g_ReadLayout), .Texture = Texture });

    return true;
}

bool ImageManager::RegisterTexture(strzilla::string_view const &Key, strzilla::string_view const &Path)
{
    std::vector<std::shared_ptr<RenderCore::Texture>> const LoadedTextures = RenderCore::Renderer::LoadImages({ Path });
    if (std::empty(LoadedTextures))
    {
        return false;
    }

    std::shared_ptr<RenderCore::Texture> Texture = LoadedTextures.at(0);
    return RegisterTexture(Key, std::move(Texture));
}

void ImageManager::RegisterTextureArray(std::unordered_map<strzilla::string_view, strzilla::string_view> &&KeysAndPaths)
{
    std::unordered_map<strzilla::string_view, strzilla::string_view> ParsedMap {};
    ParsedMap.reserve(std::size(KeysAndPaths));

    std::vector<strzilla::string_view> PathsToLoad {};
    PathsToLoad.reserve(std::size(KeysAndPaths));

    for (auto &[Key, Path] : KeysAndPaths)
    {
        if (std::empty(Key) || std::empty(Path) || m_Textures.contains(m_HashCreator(Key)))
        {
            continue;
        }

        ParsedMap.emplace(Key, Path);
        PathsToLoad.push_back(Path);
    }

    std::vector<std::shared_ptr<RenderCore::Texture>> const LoadedTextures = RenderCore::Renderer::LoadImages(std::move(PathsToLoad));

    for (auto &[Key, Path] : ParsedMap)
    {
        auto MatchingTexture = std::find_if(std::execution::unseq,
                                            std::cbegin(LoadedTextures),
                                            std::cend(LoadedTextures),
                                            [&Path](std::shared_ptr<RenderCore::Texture> const &Iterator)
                                            {
                                                return Iterator && Iterator->GetPath() == Path;
                                            });

        if (MatchingTexture != std::cend(LoadedTextures))
        {
            std::shared_ptr<RenderCore::Texture> TextureToAdd = *MatchingTexture;
            [[maybe_unused]] bool const          Result       = RegisterTexture(Key, std::move(TextureToAdd));
        }
    }
}

void ImageManager::UnregisterTexture(strzilla::string_view const &Key)
{
    if (auto const Hash = m_HashCreator(Key);
        m_Textures.contains(Hash))
    {
        if (auto &[DescriptorSet, Texture] = m_Textures.at(Hash);
            DescriptorSet != VK_NULL_HANDLE)
        {
            ImGuiVulkanRemoveTexture(DescriptorSet);
        }

        m_Textures.erase(Hash);
    }
}

void ImageManager::UnregisterAllTextures()
{
    if (std::empty(m_Textures))
    {
        return;
    }

    for (auto &[DescriptorSet, Texture] : m_Textures | std::views::values)
    {
        if (DescriptorSet != VK_NULL_HANDLE)
        {
            ImGuiVulkanRemoveTexture(DescriptorSet);
        }
    }

    m_Textures.clear();
}

void ImageManager::RefreshTextures()
{
    VkSampler const Sampler { RenderCore::GetSampler() };

    for (auto &[DescriptorSet, Texture] : m_Textures | std::views::values)
    {
        if (DescriptorSet != VK_NULL_HANDLE)
        {
            ImGuiVulkanRemoveTexture(DescriptorSet);
        }

        if (Sampler == VK_NULL_HANDLE || !Texture)
        {
            continue;
        }

        VkImageView const View = Texture->GetImageDescriptor().imageView;
        if (View == VK_NULL_HANDLE)
        {
            continue;
        }

        DescriptorSet = ImGuiVulkanAddTexture(Sampler, View, RenderCore::g_ReadLayout);
    }

    std::erase_if(m_Textures,
                  [](std::pair<std::size_t, ImageData> const &PairIt)
                  {
                      return !PairIt.second.Texture || PairIt.second.DescriptorSet == VK_NULL_HANDLE;
                  });
}
