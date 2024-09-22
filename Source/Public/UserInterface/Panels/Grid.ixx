// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include "luGUIModule.hpp"

export module luGUI.UserInterface.Panels.Grid;

import luGUI.UserInterface.Panels.Panel;
import luGUI.UserInterface.Items.Item;

namespace luGUI
{
    export class LUGUIMODULE_API Grid : public Panel<std::vector<std::shared_ptr<Item>>>, public std::enable_shared_from_this<Grid>
    {
        std::int32_t m_NumColumns { 0 };

    public:
        template <typename... Args>
        explicit constexpr Grid(Args &&... Arguments)
            : Panel(std::forward<Args>(Arguments)...)
        {
        }

        template <typename... Args>
        static constexpr std::shared_ptr<Grid> Create(Args &&... Arguments)
        {
            return Construct<Grid>(std::forward<Args>(Arguments)...);
        }

        template <typename Type>
        std::shared_ptr<Grid> Add(std::size_t const Line, std::size_t const Column, std::shared_ptr<Type> &&Item)
        {
            if (std::size(m_Items) <= Line)
            {
                m_Items.resize(Line + 1);
            }

            if (std::size(m_Items.at(Line)) <= Column)
            {
                m_Items.at(Line).resize(Column + 1);
            }

            m_Items.at(Line).at(Column) = Item;
            return shared_from_this();
        }

        template <typename Type, typename... Args>
        std::shared_ptr<Grid> Add(std::size_t const Line, std::size_t const Column, Args &&... Arguments)
        {
            return Add(Line, Column, Item::Construct<Type>(std::forward<Args>(Arguments)...));
        }

        void Draw() override;

    protected:
        void Render() override;
    };
} // namespace luGUI
