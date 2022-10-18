#ifndef _SHADER_H_
#define _SHADER_H_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>

class Shader {
private:
    GLuint program;
    GLuint shaderobj;
    GLenum type;

    std::string ReadFile(const char* path)
    {
        std::string text;
        std::ifstream fileStream(path, std::ios::in);

        if (!fileStream.is_open()) {
            std::cerr << "ERROR: Could not open file at " << path << "\n";
            return "";
        }

        std::string line = "";
        while (!fileStream.eof()) {
            getline(fileStream, line);
            text.append(line + "\n");
        }

        fileStream.close();
        return text;
    }
public:
    Shader(GLuint _type)
    {
        type = _type;
        program = glCreateProgram();
        shaderobj = 0;
    }
    ~Shader(void)
    {
        glDeleteShader(shaderobj);
        if (program) glDeleteProgram(program);
    }

    void LoadFromFile(const char* path)
    {
        // Create shader names
        GLuint shader = glCreateShader(type);

        // Get the shaders' source code;
        std::string shaderString = ReadFile(path);
        const char* shaderCode = shaderString.c_str();

        int success;
        char infoLog[512];

        // Compiles shader
        glShaderSource(shader, 1, &shaderCode, NULL);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR: Problem while compiling shader:" << "\n" << infoLog << "\n"; // TODO: Show if it is vertex or fragment shader
        }
        // Link the program
        glAttachShader(program, shader);
        glLinkProgram(program);

        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cerr << "ERROR: Problem while linking shader:\n" << infoLog << "\n";
        }

        program = program;
    }
    void LoadFromString(std::string text)
    {
        // Create shader names
        GLuint shader = glCreateShader(type);

        const char* shaderCode = text.c_str();

        int success;
        char infoLog[512];

        // Compiles shader
        glShaderSource(shader, 1, &shaderCode, NULL);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR: Problem while compiling shader:\n" << infoLog << "\n"; // TODO Show if it is vertex or fragment shader
        }
        // Link the program
        glAttachShader(program, shader);
        glLinkProgram(program);

        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cerr << "ERROR: Problem while linking shader:\n" << infoLog << "\n";
        }

        program = program;

        return;
    }
    void LoadFromBinaryFile(const char* path)
    {
        // Create shader names
        GLuint shader = glCreateShader(type);

        // Get the shaders' source code;
        std::string shaderString = ReadFile(path);
        const char* shaderCode = shaderString.c_str();

        int success;
        char infoLog[512];

        glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, shaderCode, strlen(shaderCode));

        // Specialize the shader (specify the entry point)
        glSpecializeShader(shader, "main", 0, 0, 0);


        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR: Problem while compiling shader:\n" << infoLog << "\n"; // TODO Show if it is vertex or fragment shader
        }

        // Link the program
        glAttachShader(program, shader);
        glLinkProgram(program);

        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cerr << "ERROR: Problem while linking shader:\n" << infoLog << "\n";
        }

        shaderobj = shader;
    }
    void LoadFromBinaryString(std::string text)
    {
        // Create shader names
        GLuint shader = glCreateShader(type);

        const char* shaderCode = text.c_str();

        int success;
        char infoLog[512];

        glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, shaderCode, strlen(shaderCode));

        // Specialize the shader (specify the entry point)
        glSpecializeShader(shader, "main", 0, 0, 0);


        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR: Problem while compiling shader:\n" << infoLog << "\n"; // TODO Show if it is vertex or fragment shader
        }
        // Link the program
        glAttachShader(program, shader);
        glLinkProgram(program);

        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cerr << "ERROR: Problem while linking shader:\n" << infoLog << "\n";
        }

        program = program;
    }

    void Bind() { glUseProgram(program); }
    void Unbind() { glUseProgram(0); }

    void Link()
    {
        glAttachShader(program, shaderobj);
        glLinkProgram(program);

        int success;
        char infoLog[512];

        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cerr << "ERROR: Problem while linking shader:\n" << infoLog << "\n";
        }
    }

    GLuint getProgram() const
    {
        return program;
    }

    void setProgram(GLuint _program)
    {
        glDetachShader(program, shaderobj);
        glDeleteProgram(program);
        program = _program;
    }
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
    }
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(program, name.c_str()), value);
    }
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(program, name.c_str()), value);
    }
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(program, name.c_str()), x, y);
    }
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
    }
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(program, name.c_str()), x, y, z, w);
    }
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};

#endif