// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include "luGUIModule.hpp"

export module luGUI.UserInterface.Image;

import luGUI.UserInterface.Control;

namespace luGUI
{
    export class LUGUIMODULE_API Image final : public Control
    {
        std::vector<VkDescriptorSet> m_ImageDescriptorSets{};
        bool                         m_Open{false};

    public:
        explicit Image(Control *);
        ~        Image() override;

    protected:
        void Refresh() override;
        void PrePaint() override;
        void Paint() override;
        void PostPaint() override;
    };
} // namespace luGUI
