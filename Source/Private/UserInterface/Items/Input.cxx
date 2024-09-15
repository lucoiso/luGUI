// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

module luGUI.UserInterface.Items.Input;

using namespace luGUI;

std::int32_t luGUI::InputTextCallback(ImGuiInputTextCallbackData *const Data)
{
    if (auto const UserData = static_cast<strzilla::string *>(Data->UserData))
    {
        if (std::size(*UserData) < Data->BufTextLen)
        {
            UserData->resize(Data->BufTextLen);
        }

        Data->Buf = UserData->c_str();
    }

    return 0;
}
