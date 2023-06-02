#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

#include "AppState/AppState.h"
#include "controllers/MouseController/MouseController.h"
#include "include/rapidjson/document.h"
#include "include/rapidjson/filereadstream.h"
#include "utils/utilities.h"


void MouseMoveLoop(AppState& appState) {
	auto& timePoint = appState.timePoint;
	Config& config = appState.GetConfig();
	auto configIterator = appState.GetConfig().begin();
	const auto& lastConfigIterator = config.end();

	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if (!appState.active) {
			timePoint = std::chrono::steady_clock::now();
			configIterator = config.begin();
			continue;
		};

		if (std::chrono::steady_clock::now() - timePoint < configIterator->time) MouseController::MoveMouse(configIterator->deltaX, configIterator->deltaY);
		else {
			if (configIterator + 1 != lastConfigIterator) ++configIterator;
			timePoint = std::chrono::steady_clock::now();
		}
	};
}

void MouseButtonsListener(AppState& appState) {
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(25));

		if (utilities::GetKeyState(VK_LBUTTON) && utilities::GetKeyState(VK_RBUTTON)) appState.active = true;
		else appState.active = false;
	}
}

int main() {
	// Read JSON file from path
	const std::string filePath = std::filesystem::current_path().string() + "\\data\\config.json";

	std::ifstream file(filePath);

	if (!file.is_open()) {
		std::cout << "Failed to open JSON file: " << filePath << std::endl;
		return 1;
	}

	// Read JSON contents into a string
	std::string jsonString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	// Parse JSON string
	rapidjson::Document document;
	document.Parse(jsonString.c_str());

	std::cout << document["test"].GetString() << '\n';

	const std::vector<Config> configs = {
		{
			ConfigNode(25, 25, std::chrono::milliseconds(200)),
			ConfigNode(0, 25, std::chrono::milliseconds(200)),
			ConfigNode(-25, 25, std::chrono::milliseconds(200)),
			ConfigNode(-25, 0, std::chrono::milliseconds(200)),
			ConfigNode(-25, -25, std::chrono::milliseconds(200)),
			ConfigNode(0, -25, std::chrono::milliseconds(200)),
			ConfigNode(25, -25, std::chrono::milliseconds(200)),
			ConfigNode(25, 0, std::chrono::milliseconds(200)),
			ConfigNode(0, 0, std::chrono::milliseconds(200)),
		},
		{
			ConfigNode(0, 25, std::chrono::milliseconds(400)),
			ConfigNode(0, 10, std::chrono::milliseconds(300)),
			ConfigNode(7, 10, std::chrono::milliseconds(400))
		}
	};

	AppState appState;
	appState.SetConfig(configs[0]);

	const Config& config = appState.GetConfig();

	std::thread mouseMoveThread(MouseMoveLoop, std::ref(appState));
	std::thread mouseButtonsThread(MouseButtonsListener, std::ref(appState));

	mouseMoveThread.join();
	mouseButtonsThread.join();

	return 0;
}