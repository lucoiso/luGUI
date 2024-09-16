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
        Stack() = default;
        explicit Stack(float Width);
        explicit Stack(Orientation Orientation, float Width = 0.F);
        explicit Stack(std::vector<std::shared_ptr<Item>> &&Items, float Width = 0.F);
        Stack(Orientation Orientation, std::vector<std::shared_ptr<Item>> &&Items, float Width = 0.F);

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
