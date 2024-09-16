// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include "luGUIModule.hpp"

export module luGUI.UserInterface.Items.ComboBox;

export import luGUI.UserInterface.Items.Item;

namespace luGUI
{
    export class LUGUIMODULE_API ComboBox : public Item
    {
        std::int32_t                          m_CurrentSelection = 0;
        strzilla::string                      m_Label {};
        std::vector<strzilla::string> *       m_Options { nullptr };
        std::vector<const char *>             m_InternalOptions {};
        std::function<void(strzilla::string)> m_OnChanged {};

    public:
        template <typename... Args>
        explicit constexpr ComboBox(const char *const &                     Label,
                                 std::vector<strzilla::string> *const    Options,
                                 std::function<void(strzilla::string)> &&OnChanged,
                                 Args &&...                              Arguments)
            : Item(std::forward<Args>(Arguments)...)
          , m_Label(Label)
          , m_Options(Options)
          , m_OnChanged(OnChanged)
        {
            UpdateInternalOptions();
        }

        [[nodiscard]] inline std::int32_t GetCurrentSelection() const
        {
            return m_CurrentSelection;
        }

        inline void SetCurrentSelection(std::int32_t const CurrentSelection)
        {
            m_CurrentSelection = CurrentSelection;
        }

        [[nodiscard]] inline strzilla::string_view GetLabel() const
        {
            return m_Label;
        }

        inline void SetLabel(const char *const Label)
        {
            m_Label = Label;
        }

        [[nodiscard]] inline std::vector<strzilla::string> const *GetOptions() const
        {
            return m_Options;
        }

        inline void SetOptions(std::vector<strzilla::string> *Options)
        {
            m_Options = Options;
            UpdateInternalOptions();
        }

        inline void SetOnChanged(std::function<void(strzilla::string)> &&OnChanged)
        {
            m_OnChanged = OnChanged;
        }

    protected:
        void Render() override;

    private:
        void CheckOptions();
        void UpdateInternalOptions();
    };
} // namespace luGUI
