#include "utilities.h"
#include <filesystem>

bool utilities::GetKeyState(const int vKey) {
	return (GetAsyncKeyState(vKey) & 0x8000) != 0;
}

std::map<std::string, Config> utilities::ReadJsonConfigs() {
	// Read JSON file from path

	const std::string filePath = std::filesystem::current_path().string() + "\\data\\config.json";

	std::ifstream file(filePath);
	if (!file.is_open()) throw std::runtime_error("Failed to open the file: " + filePath);

	std::string json((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	// Create a RapidJSON document
	rapidjson::Document document;
	document.Parse(json.c_str());

	std::map<std::string, Config> result;

	if (document.HasMember("profiles") && document["profiles"].IsObject()) {
		const rapidjson::Value& profiles = document["profiles"];

		for (auto iterator = profiles.MemberBegin(), end = profiles.MemberEnd(); iterator != end; ++iterator) {
			const std::string& profileName = iterator->name.GetString();
			const rapidjson::Value& profile = iterator->value;

			Config config;

			rapidjson::SizeType size = profile.Size();
			config.reserve(size);

			if (profile.IsArray()) {

				for (rapidjson::SizeType i = 0; i < size; i++) {
					const rapidjson::Value& configNode = profile[i];

					// Access the individual values within each object
					if (
						configNode.HasMember("deltaX") &&
						configNode.HasMember("deltaY") &&
						configNode.HasMember("time") &&
						configNode["deltaX"].IsInt() &&
						configNode["deltaY"].IsInt() &&
						configNode["time"].IsInt()
						) {
						int deltaX = configNode["deltaX"].GetInt();
						int deltaY = configNode["deltaY"].GetInt();
						std::chrono::milliseconds time(configNode["time"].GetInt());


						config.emplace_back(ConfigNode(deltaX, deltaY, time));
					}
				}
			}

			result.emplace(profileName, config);
		}
	}

	return result;
}
