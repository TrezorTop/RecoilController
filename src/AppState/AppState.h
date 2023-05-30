#pragma once
#include <chrono>
#include <vector>

struct ConfigNode {
  int deltaX;
  int deltaY;
  std::chrono::milliseconds time;
};

typedef std::vector<ConfigNode> Config;

class AppState {
public:
  bool isActive = false;

  void SetSettings(const Config &settings);
  Config &GetConfig();

private:
  Config config = {{0, 20, std::chrono::milliseconds(400)},
                   {0, 10, std::chrono::milliseconds(700)},
                   {3, 7, std::chrono::milliseconds(500)}};
};
