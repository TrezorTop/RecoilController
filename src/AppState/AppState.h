#pragma once
#include <chrono>
#include <vector>

struct ConfigNode {
  ConfigNode(int deltaX, int deltaY, std::chrono::milliseconds time) {
    this->deltaX = deltaX;
    this->deltaY = deltaY;
    this->time = time;
  }

  int deltaX;
  int deltaY;
  std::chrono::milliseconds time;
};

typedef std::vector<ConfigNode> Config;

class AppState {
 public:
  bool isActive = false;

  void SetConfig(const Config &config);
  Config &GetConfig();

 private:
  Config config;
};
