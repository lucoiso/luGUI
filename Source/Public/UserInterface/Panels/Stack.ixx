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
        explicit Stack(Orientation Orientation);
        explicit Stack(std::vector<std::shared_ptr<Item>> &&Items);
        Stack(Orientation Orientation, std::vector<std::shared_ptr<Item>> &&Items);

        static inline std::shared_ptr<Stack> Create()
        {
            return Construct<Stack>();
        }

        static inline std::shared_ptr<Stack> Create(Orientation const Orientation)
        {
            return Construct<Stack>(Orientation);
        }

        static inline std::shared_ptr<Stack> Create(std::vector<std::shared_ptr<Item>> &&Items)
        {
            return Construct<Stack>(std::move(Items));
        }

        static inline std::shared_ptr<Stack> Create(Orientation const Orientation, std::vector<std::shared_ptr<Item>> &&Items)
        {
            return Construct<Stack>(Orientation, std::move(Items));
        }

        template <typename Type, typename... Args>
        std::shared_ptr<Stack> Add(Args &&... Arguments)
        {
            m_Items.push_back(Item::Construct<Type>(std::forward<Args>(Arguments)...));
            return shared_from_this();
        }

        void Draw() override;
    };
} // namespace luGUI
