#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <filesystem>

namespace util
{
    const std::string FULL_PATH = std::filesystem::current_path().string();
    const std::string SHADER_PATH = FULL_PATH + "/res/shaders/";
    const std::string MODEL_PATH = FULL_PATH + "/res/models/";
    const std::string TEXTURE_PATH = FULL_PATH + "/res/textures/";

    unsigned int textureFromFile(const char *path, const std::string &directory);
}
#endif // UTIL_HPP