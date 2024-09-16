// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include "luGUIModule.hpp"

export module luGUI.UserInterface.Items.Item;

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

    export class LUGUIMODULE_API Item
    {
    protected:
        float     m_Width { 0.F };
        Alignment m_Alignment { Alignment::None };

    public:
        explicit Item(float Width = 0.F, Alignment Alignment = Alignment::None);
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

        [[nodiscard]] inline Alignment GetAlignment() const
        {
            return m_Alignment;
        }

        inline void SetAlignment(Alignment const Alignment)
        {
            m_Alignment = Alignment;
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
