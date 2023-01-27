#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <SDL.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// =============================================================================
// Shader Class
// =============================================================================
class Shader {
    public:
        void init(std::string vsFilename, std::string fsFilename);
        void _loadGLSLFromFile(GLuint& shader, GLuint type, std::string filename);

    public:
        GLuint program;
        GLuint vertexShader;
        GLuint fragmentShader;
};

// =============================================================================
// Load GLSL From File
// =============================================================================
void Shader::_loadGLSLFromFile(GLuint& shader, GLuint type, std::string filename) {

    // ensure shader type is correct
    std::string typeString = "";
    if(type == GL_VERTEX_SHADER) {
        typeString = "GL_VERTEX_SHADER";
    }
    else if(type == GL_FRAGMENT_SHADER) {
        typeString = "GL_FRAGMENT_SHADER";
    }
    else {
        std::cout << "ERROR: Shader type must be either:" << std::endl
                  << "- GL_VERTEX_SHADER" << std::endl
                  << "- GL_FRAGMENT_SHADER" << std::endl;
        exit(1);
    }

    // read shader source from file
    std::string shaderSource = "";
    std::string line = "";
    std::ifstream file(filename.c_str());
    if(file.is_open()) {
        while(std::getline(file, line)) {
            shaderSource += line + "\n";
        }
        file.close();
    }

    // load and compile shader
    shader = glCreateShader(type);
    const char* src = shaderSource.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // error check shader
    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* errorMessages = new char[length];
        glGetShaderInfoLog(shader, length, &length, errorMessages);
        std::cout << "ERROR: " << typeString << " compilation failed." << std::endl
                  << errorMessages << std::endl;
        delete[] errorMessages;
        glDeleteShader(shader);
        exit(1);
    }
}

// =============================================================================
// Construct Shader
// =============================================================================
void Shader::init(std::string vsFilename, std::string fsFilename) {

    program = glCreateProgram();

    _loadGLSLFromFile(vertexShader, GL_VERTEX_SHADER, vsFilename);
    _loadGLSLFromFile(fragmentShader, GL_FRAGMENT_SHADER, fsFilename);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glValidateProgram(program);
}

#endif // SHADER_H
