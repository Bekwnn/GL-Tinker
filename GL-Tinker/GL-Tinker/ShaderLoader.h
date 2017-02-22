#pragma once
#include <fstream>
#include <iostream>

#include <GL\glew.h>

class ShaderLoader {
public:
	static GLuint CompileVertFrag(const char* vertFileName, const char* fragFileName);
	static GLuint CompileCompute(const char* compFileName);
	static std::string ReadShader(const char* fileName);
};