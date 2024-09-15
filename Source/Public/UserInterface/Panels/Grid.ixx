// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include "luGUIModule.hpp"

export module luGUI.UserInterface.Panels.Grid;

import luGUI.UserInterface.Items.Item;

namespace luGUI
{
    export class LUGUIMODULE_API Grid : public Item, public std::enable_shared_from_this<Grid>
    {
        std::vector<std::vector<std::shared_ptr<Item>>> m_Items {};

    public:
        Grid() = default;
        explicit Grid(std::vector<std::vector<std::shared_ptr<Item>>> &&Items);

        static inline std::shared_ptr<Grid> Create()
        {
            return Construct<Grid>();
        }

        static inline std::shared_ptr<Grid> Create(std::vector<std::vector<std::shared_ptr<Item>>> &&Items)
        {
            return Construct<Grid>(std::move(Items));
        }

        template <typename Type, typename... Args>
        std::shared_ptr<Grid> Add(std::size_t const Line, std::size_t const Column, Args &&... Arguments)
        {
            if (std::size(m_Items) <= Line)
            {
                m_Items.resize(Line + 1);
            }

            if (std::size(m_Items.at(Line)) <= Column)
            {
                m_Items.at(Line).resize(Column + 1);
            }

            m_Items.at(Line).at(Column) = Item::Construct<Type>(std::forward<Args>(Arguments)...);
            return shared_from_this();
        }

        void Draw() override;
    };
} // namespace luGUI
