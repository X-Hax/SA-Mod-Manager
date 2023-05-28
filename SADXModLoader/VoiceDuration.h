#pragma once

void RegisterEnglishVoiceDuration(const uint16_t voiceID, const uint16_t duration);
void RegisterJapaneseVoiceDuration(const uint16_t voiceID, const uint16_t duration);

int GetSoundLength(std::string filename);
std::string base_name(std::string const& path);
bool isInteger(const std::string& s);