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
    protected:
        float m_Width { 0.F };

    public:
        Item() = default;
        explicit Item(float Width);
        ~Item() = default;

        virtual void Draw();

        [[nodiscard]] inline bool HasCustomWidth() const
        {
            return m_Width > 0.F;
        }

        [[nodiscard]] inline float GetWidth() const
        {
            return m_Width;
        }

        inline void SetWidth(float const Width)
        {
            m_Width = Width;
        }

        template <typename Type, typename... Args>
        static constexpr std::shared_ptr<Type> Construct(Args &&... Arguments)
        {
            return std::make_shared<Type>(std::forward<Args>(Arguments)...);
        }

    protected:
        virtual void Render()
        {
        }
    };
} // namespace luGUI
