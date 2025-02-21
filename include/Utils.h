#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <fstream>
#include <filesystem>
#include <unistd.h>

/**
 * @file Utils.h
 * @brief Header file for utility functions.
 *
 * This file defines general utility functions that are helpful throughout the application,
 * but don't belong to any specific class. Currently, it includes a function to get the executable directory.
 */

/**
 * @brief Gets the directory where the executable is located.
 *
 * This function attempts to determine the directory in which the currently running executable file is located.
 * This is often useful for finding resources (like shaders, textures, models) that are located relative to the executable.
 *
 * @return std::string The absolute path to the directory containing the executable.
 *                    Returns an empty string if the directory cannot be determined (e.g., on some systems or in error cases).
 *
 * @note  This function uses POSIX-specific system calls (`readlink`, `/proc/self/exe`) and might not be portable to non-POSIX systems (like Windows) without modification.
 * @note  It relies on the `/proc/self/exe` symbolic link available on many Unix-like systems (Linux, macOS).
 */
std::string getExecutableDir() 
{
    char buffer[1024];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1) 
    {
        buffer[len] = '\0';
        return std::filesystem::path(buffer).parent_path().string();
    }  
    return "";
    
    }
#endif // UTILS_H