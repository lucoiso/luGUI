// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/vulkan-renderer

module;

#include <imgui.h>

#ifdef GLFW_INCLUDE_VULKAN
    #undef GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

module luGUI.Integrations.ImGuiOverlay;

import RenderCore.Renderer;
import RenderCore.Runtime.Device;
import RenderCore.Runtime.SwapChain;
import RenderCore.Types.Camera;
import RenderCore.Utils.Helpers;
import RenderCore.Utils.Constants;
import luGUI.Integrations.ImGuiGLFWBackend;
import luGUI.Integrations.ImGuiVulkanBackend;

using namespace luGUI;

VkDescriptorPool g_ImGuiDescriptorPool { VK_NULL_HANDLE };

void luGUI::InitializeImGuiContext(GLFWwindow *const Window, bool const EnableDocking, bool const EnableViewports)
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO &ImIO = ImGui::GetIO();
    (void)ImIO;

    if (ImIO.BackendPlatformUserData)
    {
        return;
    }

    ImIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    if (EnableViewports)
    {
        ImIO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    }

    if (EnableDocking)
    {
        ImIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }

    ImGuiGLFWInitForVulkan(Window, false);

    RenderCore::Renderer::DispatchToMainThread([Window]
    {
        ImGuiGLFWSetCallbacksChainForAllWindows(true);
        ImGuiGLFWInstallCallbacks(Window);
    });

    constexpr std::uint32_t DescriptorCount = 100U;

    constexpr std::array DescriptorPoolSizes {
            VkDescriptorPoolSize { VK_DESCRIPTOR_TYPE_SAMPLER, DescriptorCount },
            VkDescriptorPoolSize { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DescriptorCount },
            VkDescriptorPoolSize { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, DescriptorCount },
            VkDescriptorPoolSize { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, DescriptorCount },
            VkDescriptorPoolSize { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, DescriptorCount },
            VkDescriptorPoolSize { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, DescriptorCount },
            VkDescriptorPoolSize { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, DescriptorCount },
            VkDescriptorPoolSize { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DescriptorCount },
            VkDescriptorPoolSize { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, DescriptorCount },
            VkDescriptorPoolSize { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, DescriptorCount },
            VkDescriptorPoolSize { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, DescriptorCount }
    };

    VkDescriptorPoolCreateInfo const DescriptorPoolCreateInfo {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
            .maxSets = static_cast<std::uint32_t>(std::size(DescriptorPoolSizes)),
            .poolSizeCount = std::size(DescriptorPoolSizes),
            .pPoolSizes = std::data(DescriptorPoolSizes)
    };

    RenderCore::CheckVulkanResult(vkCreateDescriptorPool(RenderCore::GetLogicalDevice(), &DescriptorPoolCreateInfo, nullptr, &g_ImGuiDescriptorPool));

    static std::vector const ColorAttachmentFormat { RenderCore::GetSwapChainImageFormat() };

    ImGuiVulkanInitInfo const ImGuiVulkanInitInfo {
            .DescriptorPool = g_ImGuiDescriptorPool,
            .PipelineRenderingCreateInfo = {
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
                    .colorAttachmentCount = static_cast<std::uint32_t>(std::size(ColorAttachmentFormat)),
                    .pColorAttachmentFormats = std::data(ColorAttachmentFormat),
                    .depthAttachmentFormat = VK_FORMAT_UNDEFINED,
                    .stencilAttachmentFormat = VK_FORMAT_UNDEFINED
            }
    };

    ImGuiVulkanInit(ImGuiVulkanInitInfo);
    ImGuiVulkanCreateFontsTexture();

    glfwPostEmptyEvent();
}

void luGUI::ReleaseImGuiResources()
{
    ImGuiVulkanShutdown();
    ImGuiGLFWShutdown();

    ImGui::DestroyContext();

    if (g_ImGuiDescriptorPool != VK_NULL_HANDLE)
    {
        VkDevice const &LogicalDevice = RenderCore::GetLogicalDevice();
        vkDestroyDescriptorPool(LogicalDevice, g_ImGuiDescriptorPool, nullptr);
        g_ImGuiDescriptorPool = VK_NULL_HANDLE;
    }
}

void luGUI::DrawImGuiFrame(Control *Control)
{
    if (!ImGui::GetCurrentContext() || !IsImGuiInitialized() || ImGui::GetIO().DisplaySize.x <= 0.F || ImGui::GetIO().DisplaySize.y <= 0.F)
    {
        return;
    }

    Control->PreUpdate();
    {
        ImGuiVulkanNewFrame();
        ImGuiGLFWNewFrame();

        ImGui::NewFrame();
        {
            Control->Update();
        }
        ImGui::Render();

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable && !std::empty(ImGui::GetPlatformIO().Viewports))
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }
    Control->PostUpdate();
}

bool luGUI::IsImGuiInitialized()
{
    return g_ImGuiDescriptorPool != VK_NULL_HANDLE;
}

void luGUI::RecordImGuiCommandBuffer(VkCommandBuffer const &CommandBuffer, RenderCore::ImageAllocation const &SwapchainAllocation)
{
    if (IsImGuiInitialized())
    {
        if (ImDrawData *const ImGuiDrawData = ImGui::GetDrawData())
        {
            VkRenderingAttachmentInfo const ColorAttachmentInfo {
                    .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
                    .imageView = SwapchainAllocation.View,
                    .imageLayout = RenderCore::g_AttachmentLayout,
                    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                    .clearValue = RenderCore::g_ClearValues.at(0U)
            };

            VkRenderingInfo const RenderingInfo {
                    .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
                    .flags = VK_RENDERING_CONTENTS_SECONDARY_COMMAND_BUFFERS_BIT,
                    .renderArea = { .offset = { 0, 0 }, .extent = SwapchainAllocation.Extent },
                    .layerCount = 1U,
                    .colorAttachmentCount = 1U,
                    .pColorAttachments = &ColorAttachmentInfo,
            };

            vkCmdBeginRendering(CommandBuffer, &RenderingInfo);
            ImGuiVulkanRenderDrawData(ImGuiDrawData, CommandBuffer);
            vkCmdEndRendering(CommandBuffer);
        }
    }
}
