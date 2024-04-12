#ifndef SHADER_H
#define SHADER_H


#include <glm/glm.hpp>
#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

#if __cplusplus >= 201703L
#include <filesystem>
#endif


class Shader
{
public:
   Shader() {
}

    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try 
        {


            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);

            
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();	

    
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();	
		
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        
        unsigned int vertex, fragment;

        std::cout<<"problem starts hre\n";
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);


    }
    //build function 
void build(const std::string& vertexFilepath, const std::string& fragmentFilepath)
{
    std::string vertexSource = readFileContents(vertexFilepath); 
    std::string fragmentSource = readFileContents(fragmentFilepath); 

    const GLchar* vsrc = vertexSource.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vsrc, NULL);
    glCompileShader(vertexShader);

    checkShaderError(vertexShader, "Vertex Shader");

    const GLchar* fsrc = fragmentSource.c_str();

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fsrc, NULL);
    glCompileShader(fragmentShader);

    checkShaderError(fragmentShader, "Fragment Shader");

    // link shaders
    programId = glCreateProgram();
    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    glLinkProgram(programId);
    
    // check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        std::cerr << "Shader program linking failed...\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

    // activate the shader
    // ------------------------------------------------------------------------
    void use() const
    { 
        glUseProgram(ID); 
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) const
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    template<typename T>
    void setVar(T var, std::string varname)
    {
        GLint location = glGetUniformLocation(programId, varname.c_str());
        glUniform1f(location, var);
    }

    template <>
    void setVar<glm::mat4>(glm::mat4 var, std::string varname)
    {
        GLint location = glGetUniformLocation(programId, varname.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, &var[0][0]);
    }

    template <>
    void setVar<glm::vec3>(glm::vec3 var, std::string varname)
    {
        GLint location = glGetUniformLocation(programId, varname.c_str());
        glUniform3fv(location, 1, &var[0]);
    }

    template <>
    void setVar<glm::vec2>(glm::vec2 var, std::string varname)
    {
        GLint location = glGetUniformLocation(programId, varname.c_str());
        glUniform2fv(location, 1, &var[0]);
    }


private:
    GLuint programId;

    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
    //check Shader Error
    void checkShaderError(GLuint shaderId, std::string type)
    {
        int success;
        char infoLog[512];
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
            std::cerr << type << " compilation error...\n" << infoLog << std::endl;
        }
    }
    std::string readFileContents(const std::string& filepath)
    {
    #if __cplusplus >= 201703L
        if(!std::filesystem::exists(filepath))
        {
            std::stringstream error;
            error << "File read error: " << std::filesystem::current_path() / filepath << " not found.";
            throw std::runtime_error(error.str());
        }
    #endif

        std::string contents;
        std::ifstream infile(filepath, std::ios::ate);

    #if __cplusplus < 201703L
        if(!infile.is_open())
        {
            std::stringstream error;
            error << "File read error: " << filepath << " not found.";
            throw std::runtime_error(error.str());
        }
    #endif

        std::ios::pos_type end = infile.tellg();
        contents.resize(end);
        infile.seekg(0, std::ios::beg);
        infile.read(&contents[0], end);
        infile.close();

        return contents;
    }

};
#endif
