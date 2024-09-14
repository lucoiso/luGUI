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
        inline virtual void Draw() const
        {
        }
    };
} // namespace luGUI
