#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader{
    public:
        // id of the program
        unsigned int ID;

        // constructor
        Shader(const char* vertexPath, const char* fragmentPath);

        // initialize program
        void initialize();

        // utility functions
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;

};



#endif // SHADER_H