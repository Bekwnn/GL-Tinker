#pragma once
#include "Actor.h"

#include <assimp\scene.h>
#include "AssimpUtil.h"

class MeshActor : public Actor
{
public:
	MeshActor();
	virtual void Render() override;
	virtual void ShadowPass() override;
	virtual void GenBuffers();
	virtual void FreeBuffers();
	virtual void SetUniforms();
	virtual void SetBufferData();
	virtual void DrawBuffers();
	virtual void CleanUp();
	std::string SetMesh(std::string& pFile);
	void SetShader(const char* vertProgram, const char* fragProgram);
	void SetShadowShader(const char* vertProgram, const char* fragProgram);

	aiu::Mesh mesh;
	glm::mat4 model;
	glm::mat4 depthBiasMVP;
	static glm::mat4 biasMatrix;

	GLuint vao;
	GLuint shaderProgram;
	GLuint shadowProgram;
	GLuint elementBuffer;
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint tangentBuffer;
	GLuint bitangentBuffer;

	bool normalMapped;
	bool castsShadow;
	bool isShadowed;
};