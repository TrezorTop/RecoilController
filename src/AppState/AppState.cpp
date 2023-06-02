#include "AppState.h"

void AppState::SetConfig(const Config& config) {
	this->config = config;
}

Config& AppState::GetConfig() {
	return this->config;
}