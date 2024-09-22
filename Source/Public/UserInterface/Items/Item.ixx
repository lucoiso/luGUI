// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include "luGUIModule.hpp"

#include <imgui.h>

export module luGUI.UserInterface.Items.Item;

namespace luGUI
{
    export class LUGUIMODULE_API Item
    {
    protected:
        float  m_Width { 0.F };
        ImVec2 m_ItemSize { 0.F, 0.F };

    public:
        Item()  = default;
        ~Item() = default;

        explicit Item(float Width);

        virtual void Draw();

        [[nodiscard]] inline bool HasCustomWidth() const
        {
            return m_Width > 0.F;
        }

        [[nodiscard]] inline float GetWidth() const
        {
            return m_Width;
        }

        [[nodiscard]] inline ImVec2 const &GetItemSize() const
        {
            return m_ItemSize;
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
