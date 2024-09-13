// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

#ifndef LUGUIMODULE_H
#define LUGUIMODULE_H

#ifdef BUILD_DLL
#define LUGUIMODULE_API _declspec(dllexport)
#else
#define LUGUIMODULE_API _declspec(dllimport)
#endif
#endif
