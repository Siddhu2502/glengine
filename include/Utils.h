#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <fstream>
#include <filesystem>
#include <unistd.h>

std::string getExecutableDir() {
    char buffer[1024];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1) {
        buffer[len] = '\0';
        return std::filesystem::path(buffer).parent_path().string();
    }
    return "";
}

#endif // UTILS_H