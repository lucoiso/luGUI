// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include "luGUIModule.hpp"

export module luGUI.UserInterface.Viewport;

import luGUI.UserInterface.Control;

namespace luGUI
{
    export class LUGUIMODULE_API Viewport final : public Control
    {
        std::vector<VkDescriptorSet> m_ViewportDescriptorSets{};
        bool                         m_Open{false};

    public:
        explicit Viewport(Control *);
        ~Viewport() override;

    protected:
        void Refresh() override;
        void PrePaint() override;
        void Paint() override;
        void PostPaint() override;
    };
} // namespace luGUI
