// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include "luGUIModule.hpp"

export module luGUI.UserInterface.Panels.Stack;

import luGUI.UserInterface.Panels.Panel;
import luGUI.UserInterface.Items.Item;

namespace luGUI
{
    export enum class Orientation
    {
        Horizontal,
        Vertical
    };

    export class LUGUIMODULE_API Stack : public Panel<std::shared_ptr<Item>>, public std::enable_shared_from_this<Stack>
    {
        Orientation m_Orientation { Orientation::Horizontal };

    public:
        template <typename... Args>
        explicit constexpr Stack(Orientation const Orientation, Args &&... Arguments)
            : Panel(std::forward<Args>(Arguments)...)
          , m_Orientation(Orientation)
        {
        }

        template <typename... Args>
        static constexpr std::shared_ptr<Stack> Create(Args &&... Arguments)
        {
            return Construct<Stack>(std::forward<Args>(Arguments)...);
        }

        template <typename Type>
        std::shared_ptr<Stack> Add(std::shared_ptr<Type> &&Item)
        {
            m_Items.push_back(std::move(Item));
            return shared_from_this();
        }

        template <typename Type, typename... Args>
        std::shared_ptr<Stack> Add(Args &&... Arguments)
        {
            return Add(Item::Construct<Type>(std::forward<Args>(Arguments)...));
        }

        void Draw() override;

    protected:
        void Render() override;
    };
} // namespace luGUI
