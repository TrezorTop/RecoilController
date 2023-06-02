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

void MouseMoveLoop(std::chrono::steady_clock::time_point &startTime,
                   Config::iterator &configIterator,
                   Config::iterator &lastConfigNode, Config &config) {
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(25));
    if (!utilities::GetKeyState(VK_LBUTTON) ||
        !utilities::GetKeyState(VK_RBUTTON)) {
      startTime = std::chrono::steady_clock::now();
      configIterator = config.begin();
      continue;
    };

    std::chrono::milliseconds duration = configIterator->time;

    if (std::chrono::steady_clock::now() - startTime < duration)
      MouseController::MoveMouse(configIterator->deltaX,
                                 configIterator->deltaY);
    else {
      if (configIterator + 1 != lastConfigNode)
        ++configIterator;
      startTime = std::chrono::steady_clock::now();
    }
  };
}

int main() {
  // Read JSON file from path
  const std::string filePath =
      std::filesystem::current_path().string() + "\\data\\config.json";

  std::ifstream file(filePath);

  if (!file.is_open()) {
    std::cout << "Failed to open JSON file: " << filePath << std::endl;
    return 1;
  }

  // Read JSON contents into a string
  std::string jsonString((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
  file.close();

  // Parse JSON string
  rapidjson::Document document;
  document.Parse(jsonString.c_str());

  std::cout << document["test"].GetString() << '\n';

  std::vector<Config> configs = {
      {ConfigNode(0, 25, std::chrono::milliseconds(400)),
       ConfigNode(0, 10, std::chrono::milliseconds(300)),
       ConfigNode(7, 10, std::chrono::milliseconds(400))},

      {ConfigNode(0, 25, std::chrono::milliseconds(400)),
       ConfigNode(0, 10, std::chrono::milliseconds(300)),
       ConfigNode(7, 10, std::chrono::milliseconds(400))}};

  AppState appState;
  appState.SetConfig(configs[0]);

  Config &config = appState.GetConfig();

  auto startTime = std::chrono::steady_clock::now();
  auto configIterator = config.begin();
  auto lastConfigNode = config.end();

  std::thread mouseThread(MouseMoveLoop, std::ref(startTime),
                          std::ref(configIterator), std::ref(lastConfigNode),
                          std::ref(config));

  mouseThread.join();

  return 0;
}