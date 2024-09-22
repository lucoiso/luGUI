// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include "luGUIModule.hpp"

#include <imgui.h>

export module luGUI.UserInterface.Panels.Panel;

import luGUI.UserInterface.Items.Item;

namespace luGUI
{
    export enum class Alignment
    {
        None,
        Left,
        TopLeft,
        Top,
        TopRight,
        Right,
        BottomLeft,
        Bottom,
        BottomRight,
        Center
    };

    export template <typename ItemType>
    class LUGUIMODULE_API Panel : public Item
    {
    protected:
        Alignment             m_Alignment { Alignment::None };
        std::vector<ItemType> m_Items {};

    public:
        template <typename... Args>
        explicit constexpr Panel(Args &&... Arguments)
            : Item(std::forward<Args>(Arguments)...)
        {
        }

        template <typename... Args>
        explicit constexpr Panel(Alignment const Alignment, Args &&... Arguments)
            : Item(std::forward<Args>(Arguments)...)
          , m_Alignment(Alignment)
        {
        }

        template <typename... Args>
        explicit constexpr Panel(std::vector<ItemType> &&Items, Args &&... Arguments)
            : Item(std::forward<Args>(Arguments)...)
          , m_Items(Items)
        {
        }

        template <typename... Args>
        explicit constexpr Panel(Alignment const Alignment, std::vector<ItemType> &&Items, Args &&... Arguments)
            : Item(std::forward<Args>(Arguments)...)
          , m_Alignment(Alignment)
          , m_Items(Items)
        {
        }

        [[nodiscard]] inline Alignment GetAlignment() const
        {
            return m_Alignment;
        }

        inline void SetAlignment(Alignment const Alignment)
        {
            m_Alignment = Alignment;
        }

        [[nodiscard]] inline bool IsEmpty() const
        {
            return std::empty(m_Items);
        }

        [[nodiscard]] inline std::size_t GetNumItems() const
        {
            return std::size(m_Items);
        }

    protected:
        void SetupAlignment()
        {
            if (m_ItemSize.x == 0.F || m_ItemSize.y == 0.F || m_Alignment == Alignment::None)
            {
                return;
            }

            m_ItemSize.x = std::truncf(m_ItemSize.x * 1.13F); // TODO : Fix incorrect offset
            m_ItemSize.y = std::truncf(m_ItemSize.y * 1.20F); // TODO : Fix incorrect offset

            ImVec2 const AvailableSpace = ImGui::GetContentRegionAvail();
            ImVec2       AlignmentPosition;

            switch (m_Alignment)
            {
                case Alignment::Left:
                {
                    AlignmentPosition = ImVec2 { 0.F, (AvailableSpace.y - m_ItemSize.y) * 0.5F };
                    break;
                }
                case Alignment::TopLeft:
                {
                    AlignmentPosition = ImVec2 { 0.F, 0.F };
                    break;
                }
                case Alignment::Top:
                {
                    AlignmentPosition = ImVec2 { (AvailableSpace.x - m_ItemSize.x) * 0.5F, 0 };
                    break;
                }
                case Alignment::TopRight:
                {
                    AlignmentPosition = ImVec2 { AvailableSpace.x - m_ItemSize.x, 0.F };
                    break;
                }
                case Alignment::Right:
                {
                    AlignmentPosition = ImVec2 { AvailableSpace.x - m_ItemSize.x, (AvailableSpace.y - m_ItemSize.y) * 0.5F };
                    break;
                }
                case Alignment::BottomLeft:
                {
                    AlignmentPosition = ImVec2 { 0.F, AvailableSpace.y - m_ItemSize.y };
                    break;
                }
                case Alignment::Bottom:
                {
                    AlignmentPosition = ImVec2 { (AvailableSpace.x - m_ItemSize.x) * 0.5F, AvailableSpace.y - m_ItemSize.y };
                    break;
                }
                case Alignment::BottomRight:
                {
                    AlignmentPosition = ImVec2 { AvailableSpace.x - m_ItemSize.x, AvailableSpace.y - m_ItemSize.y };
                    break;
                }
                case Alignment::Center:
                {
                    AlignmentPosition = ImVec2 { (AvailableSpace.x - m_ItemSize.x) * 0.5F, (AvailableSpace.y - m_ItemSize.y) * 0.5F };
                    break;
                }
                case Alignment::None: default:
                    std::unreachable();
            }

            ImGui::SetCursorPos(AlignmentPosition);
        }
    };
} // namespace luGUI
