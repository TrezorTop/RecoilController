#include "AppState.h"
void AppState::SetSettings(const Config &settings) { this->config = settings; }
Config &AppState::GetConfig() { return this->config; }