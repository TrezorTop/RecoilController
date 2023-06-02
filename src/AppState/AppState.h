#pragma once
#include <chrono>
#include <vector>

struct ConfigNode {
	ConfigNode(const int deltaX, const int deltaY, const std::chrono::milliseconds time) {
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
	bool active = false;

	std::chrono::steady_clock::time_point timePoint = std::chrono::steady_clock::now();

	void SetConfig(const Config& config);

	Config& GetConfig();

private:
	Config config;
};
