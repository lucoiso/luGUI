// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/vulkan-renderer

module;

export module luGUI.Integrations.ImGuiOverlay;

import luGUI.UserInterface.Controls.Control;
import RenderCore.Types.SurfaceProperties;
import RenderCore.Types.Allocation;

namespace luGUI
{
    export void               InitializeImGuiContext(GLFWwindow *, bool, bool);
    export void               ReleaseImGuiResources();
    export void               DrawImGuiFrame(Control *);
    export [[nodiscard]] bool IsImGuiInitialized();
    export void               RecordImGuiCommandBuffer(VkCommandBuffer const &, RenderCore::ImageAllocation const &);
} // namespace RenderCore
