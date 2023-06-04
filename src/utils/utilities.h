#pragma once
#include <windows.h>
#include <fstream>
#include <filesystem>
#include <map>

#include "../include/rapidjson/document.h"
#include "../include/rapidjson/filereadstream.h"
#include "../AppState/AppState.h"

namespace utilities {
	bool GetKeyState(const int vKey);

	std::map<std::string, Config> ReadJsonConfigs();
}  // namespace utilities