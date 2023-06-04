#include <thread>
#include <vector>
#include <map>
#include <iostream>

#include "AppState/AppState.h"
#include "controllers/MouseController/MouseController.h"
#include "include/rapidjson/document.h"
#include "include/rapidjson/filereadstream.h"
#include "utils/utilities.h"


void MouseMoveLoop(AppState& appState) {
	auto& timePoint = appState.timePoint;
	Config& config = appState.GetConfig();
	auto configIterator = config.begin();
	const auto& lastConfigIterator = config.end();

	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if (!appState.active) {
			timePoint = std::chrono::steady_clock::now();
			configIterator = config.begin();
			continue;
		};

		bool lastItem = configIterator + 1 == lastConfigIterator;

		if (lastItem || std::chrono::steady_clock::now() - timePoint < configIterator->time)
			MouseController::MoveMouse(configIterator->deltaX, configIterator->deltaY);
		else {
			if (!lastItem) ++configIterator;
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
	const std::map<std::string, Config> configs = utilities::ReadJsonConfigs();

	if (configs.empty()) {
		std::cout << "No configurations found." << '\n';
		return ERROR;
	}

	AppState appState;

	auto it = configs.find("profile1");
	if (it != configs.end()) {
		appState.SetConfig(it->second);
	}
	else {
		std::cout << "Could't find config" << '\n';
	}

	std::thread mouseMoveThread(MouseMoveLoop, std::ref(appState));
	std::thread mouseButtonsThread(MouseButtonsListener, std::ref(appState));

	try {
		mouseMoveThread.join();
		mouseButtonsThread.join();
	}
	catch (const std::exception& e) {
		std::cout << "Threads could't join" << '\n';
		return ERROR;
	}

	return 0;
}