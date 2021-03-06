#include "Engine/Core/Config.hpp"

#include "Engine/Core/ArgumentParser.hpp"
#include "Engine/Core/KeyValueParser.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"

#include <algorithm>
#include <locale>
#include <sstream>

Config::Config(KeyValueParser&& kvp)
    : _config(std::move(kvp.Release()))
{
    /* DO NOTHING */
}

bool Config::HasKey(const std::string& key) const {
    return _config.find(key) != _config.end();
}

void Config::GetValue(const std::string& key, char& value) {
    auto found = _config.find(key);
    if(found != _config.end()) {
        value = *(found->second.begin());
    }
}

void Config::GetValue(const std::string& key, unsigned char& value) {
    auto found = _config.find(key);
    if(found != _config.end()) {
        value = static_cast<unsigned char>(std::stoul(found->second));
    }
}

void Config::GetValue(const std::string& key, signed char& value) {
    auto found = _config.find(key);
    if(found != _config.end()) {
        value = static_cast<signed char>(std::stoi(found->second));
    }
}

void Config::GetValue(const std::string& key, bool& value) {
    auto found = _config.find(key);
    if(found != _config.end()) {
        int keyAsInt = -9999;
        try {
            keyAsInt = std::stoi(found->second);
            value = keyAsInt != 0;
        } catch(...) {
            std::string keyAsString = StringUtils::ToLowerCase(found->second);
            if(found->second == "true") {
                value = true;
            } else if(found->second == "false") {
                value = false;
            } else {
                value = false;
            }
        }
    }
}

void Config::GetValue(const std::string& key, unsigned int& value) {
    auto found = _config.find(key);
    if(found != _config.end()) {
        value = static_cast<unsigned int>(std::stoul(found->second));
    }
}

void Config::GetValue(const std::string& key, int& value) {
    auto found = _config.find(key);
    if(found != _config.end()) {
        value = std::stoi(found->second);
    }
}

void Config::GetValue(const std::string& key, long& value) {
    auto found = _config.find(key);
    if(found != _config.end()) {
        value = std::stol(found->second);
    }
}

void Config::GetValue(const std::string& key, unsigned long& value) {
    auto found = _config.find(key);
    if(found != _config.end()) {
        value = std::stoul(found->second);
    }
}

void Config::GetValue(const std::string& key, long long& value) {
    auto found = _config.find(key);
    if(found != _config.end()) {
        value = std::stoll(found->second);
    }
}

void Config::GetValue(const std::string& key, unsigned long long& value) {
    auto found = _config.find(key);
    if(found != _config.end()) {
        value = std::stoull(found->second);
    }
}

void Config::GetValue(const std::string& key, float& value) {
    auto found = _config.find(key);
    if(found != _config.end()) {
        value = std::stof(found->second);
    }
}

void Config::GetValue(const std::string& key, double& value) {
    auto found = _config.find(key);
    if(found != _config.end()) {
        value = std::stod(found->second);
    }
}

void Config::GetValue(const std::string& key, long double& value) {
    auto found = _config.find(key);
    if(found != _config.end()) {
        value = std::stold(found->second);
    }
}

void Config::GetValue(const std::string& key, std::string& value) {
    auto found = _config.find(key);
    if(found != _config.end()) {
        value = found->second;
    }
}

void Config::SetValue(const std::string& key, const char& value) {
    _config[key] = value;
}

void Config::SetValue(const std::string& key, const unsigned char& value) {
    _config[key] = value;
}

void Config::SetValue(const std::string& key, const signed char& value) {
    _config[key] = value;
}

void Config::SetValue(const std::string& key, const bool& value) {
    _config[key] = value ? "true" : "false";
}

void Config::SetValue(const std::string& key, const unsigned int& value) {
    _config[key] = std::to_string(value);
}

void Config::SetValue(const std::string& key, const int& value) {
    _config[key] = std::to_string(value);
}

void Config::SetValue(const std::string& key, const long& value) {
    _config[key] = std::to_string(value);
}

void Config::SetValue(const std::string& key, const unsigned long& value) {
    _config[key] = std::to_string(value);
}

void Config::SetValue(const std::string& key, const long long& value) {
    _config[key] = std::to_string(value);
}

void Config::SetValue(const std::string& key, const unsigned long long& value) {
    _config[key] = std::to_string(value);
}

void Config::SetValue(const std::string& key, const float& value) {
    _config[key] = std::to_string(value);
}

void Config::SetValue(const std::string& key, const double& value) {
    _config[key] = std::to_string(value);
}

void Config::SetValue(const std::string& key, const long double& value) {
    _config[key] = std::to_string(value);
}

void Config::SetValue(const std::string& key, const std::string& value) {
    _config[key] = value;
}

void Config::SetValue(const std::string& key, const char* value) {
    SetValue(key, std::string(value));
}

void Config::PrintConfigs(std::ostream& output /*= std::cout*/) const {
    for(auto iter = _config.begin(); iter != _config.end(); ++iter) {
        bool value_has_space = iter->second.find(' ') != std::string::npos;
        output << iter->first << '=';
        if(value_has_space) {
            output << '"';
        }
        output << iter->second;
        if(value_has_space) {
            output << '"';
        }
        output << '\n';
    }
}

std::ostream& operator<<(std::ostream& output, const Config& config) {
    config.PrintConfigs(output);
    return output;
}

std::istream& operator>>(std::istream& input, Config& config) {
    KeyValueParser kvp(input);
    config._config = std::move(kvp.Release());
    return input;
}