// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include "luGUIModule.hpp"

export module luGUI.UserInterface.Items.Item;

namespace luGUI
{
    export class LUGUIMODULE_API Item
    {
    public:
        Item()  = default;
        ~Item() = default;

        virtual void Draw()
        {
        }

        template <typename Type, typename... Args>
        static constexpr std::shared_ptr<Type> Construct(Args &&... Arguments)
        {
            return std::make_shared<Type>(std::forward<Args>(Arguments)...);
        }
    };
} // namespace luGUI
