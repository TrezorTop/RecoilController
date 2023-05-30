#include <thread>

#include "AppState/AppState.h"
#include "controllers/MouseController/MouseController.h"
#include "utils/utilities.h"

int main() {
  AppState appState;

  Config& config = appState.GetConfig();

  auto startTime = std::chrono::steady_clock::now();
  auto configIterator = config.begin();
  auto lastConfigNode = config.end();

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
      if (configIterator + 1 != lastConfigNode) ++configIterator;
      startTime = std::chrono::steady_clock::now();
    }
  }

  return 0;
}