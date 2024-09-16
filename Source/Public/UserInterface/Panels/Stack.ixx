// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include "luGUIModule.hpp"

export module luGUI.UserInterface.Panels.Stack;

import luGUI.UserInterface.Items.Item;

namespace luGUI
{
    export enum class Orientation
    {
        Horizontal,
        Vertical
    };

    export class LUGUIMODULE_API Stack : public Item, public std::enable_shared_from_this<Stack>
    {
        Orientation                        m_Orientation { Orientation::Horizontal };
        std::vector<std::shared_ptr<Item>> m_Items {};

    public:
        template <typename... Args>
        explicit constexpr Stack(Args &&... Arguments)
            : Item(std::forward<Args>(Arguments)...)
        {
        }

        template <typename... Args>
        explicit constexpr Stack(Orientation const Orientation, Args &&... Arguments)
            : Item(std::forward<Args>(Arguments)...)
          , m_Orientation(Orientation)
        {
        }

        template <typename... Args>
        explicit constexpr Stack(std::vector<std::shared_ptr<Item>> &&Items, Args &&... Arguments)
            : Item(std::forward<Args>(Arguments)...)
          , m_Items(Items)
        {
        }

        template <typename... Args>
        explicit constexpr Stack(Orientation const Orientation, std::vector<std::shared_ptr<Item>> &&Items, Args &&... Arguments)
            : Item(std::forward<Args>(Arguments)...)
          , m_Orientation(Orientation)
          , m_Items(Items)
        {
        }

        template <typename... Args>
        static constexpr std::shared_ptr<Stack> Create(Args &&... Arguments)
        {
            return Construct<Stack>(std::forward<Args>(Arguments)...);
        }

        template <typename Type, typename... Args>
        std::shared_ptr<Stack> Add(Args &&... Arguments)
        {
            m_Items.push_back(Item::Construct<Type>(std::forward<Args>(Arguments)...));
            return shared_from_this();
        }

        void Draw() override;

    protected:
        void Render() override;
    };
} // namespace luGUI
