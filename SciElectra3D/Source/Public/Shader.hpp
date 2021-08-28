#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <glad/glad.h> 


inline size_t getSize(std::ifstream& file) {
	file.seekg(0, std::ios::end);
	size_t length = file.tellg();
	file.seekg(0, std::ios::beg);
	return length;
}
inline void readFile(char* buffer, size_t length, std::ifstream& file) {
	file.seekg(0, std::ios::beg);
	file.read(buffer, length);
	file.close();
	buffer[length] = '\0';
}
class Shader
{
public:
	Shader(const char* vsName, const char* fsName);
	Shader(const char* vsName, const char* fsName,bool aftInt);
	~Shader();
	GLint compile();
	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w);
	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;
	//void setMatrix4fv(const char* name, float& model);
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;
	bool vertexIsReady = false, fragmentIsReady = false, shaderInitialized = false, programIsReady = false,compiled = false;

private:
	char* vsSource = NULL;
	char* fsSource = NULL;
};

inline Shader::Shader(const char* _vsName, const char* _fsName)
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	program = glCreateProgram();
	programIsReady = true;
	shaderInitialized = true;
	std::ifstream vsf(_vsName, std::ios::binary);
	if(vsf.good()){
		size_t sos = getSize(vsf);
		vsSource = new char[sos];
		readFile(vsSource, sos, vsf);
		vertexIsReady = true;
	}
	std::ifstream fsf(_fsName, std::ios::binary);
	if(fsf.good()){
		size_t sos = getSize(fsf);
		fsSource = new char[sos];
		readFile(fsSource, sos, fsf);
		fragmentIsReady = true;
	}


}

inline Shader::Shader(const char* vsName, const char* fsName, bool aftInt)
{
	std::ifstream vsf(vsName, std::ios::binary);
	if(vsf.good()){
		size_t sos = getSize(vsf);
		vsSource = new char[sos];
		readFile(vsSource, sos, vsf);
		vertexIsReady = true;
	}
	std::ifstream fsf(fsName, std::ios::binary);
	if(fsf.good()){
		size_t sos = getSize(fsf);
		fsSource = new char[sos];
		readFile(fsSource, sos, fsf);
		fragmentIsReady = true;
	}
}

inline Shader::~Shader()
{
}

inline GLint Shader::compile()
{
	if (!shaderInitialized) {
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		programIsReady = glCreateProgram();
		vertexIsReady = true;
		fragmentIsReady = true;
		programIsReady = true;
	}
	vertexIsReady = true;
	fragmentIsReady = true;
	programIsReady = true;
	if (!vertexIsReady || !fragmentIsReady || !programIsReady)
		return GL_FALSE;
	GLint s1, s2, maxLen;
	
	glShaderSource(vertexShader, 1, &vsSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &s1);
	if (s1 == GL_FALSE) {
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLen);
		std::vector<GLchar> errorLog1(maxLen);
		glGetShaderInfoLog(vertexShader, maxLen, &maxLen, &errorLog1[0]);
		std::cout << "Error On Vertex Shader :";
		for (auto i = errorLog1.begin(); i != errorLog1.end(); ++i)
			std::cout << *i;
		std::cout << std::endl;
		glDeleteShader(vertexShader);
		return GL_FALSE;
	}


	glShaderSource(fragmentShader, 1, &fsSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &s2);

	if (s2 == GL_FALSE) {
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLen);
		std::vector<GLchar> errorLog2(maxLen);
		glGetShaderInfoLog(fragmentShader, maxLen, &maxLen, &errorLog2[0]);
		std::cout << "Error On Fragment Shader : ";
		for (auto i = errorLog2.begin(); i != errorLog2.end(); ++i)
			std::cout << *i;
		std::cout << std::endl;
		glDeleteShader(fragmentShader);
		return GL_FALSE;
	}

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	compiled = true;
	return GL_TRUE;

}
inline void Shader::use()
{
	glUseProgram(program);
}
inline void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
inline void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}
// ------------------------------------------------------------------------
inline void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}
// ------------------------------------------------------------------------
inline void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}
inline void Shader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(program, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
inline void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}
inline void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
inline void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}
inline void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(program, name.c_str()), x, y, z, w);
}
// 

inline void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
inline void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
inline void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
