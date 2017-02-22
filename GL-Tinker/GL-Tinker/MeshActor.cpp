#include "MeshActor.h"

#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "ShaderLoader.h"
#include "Scene.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

glm::mat4 MeshActor::biasMatrix = glm::mat4(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
	);

MeshActor::MeshActor()
{
	scale = glm::vec3(1, 1, 1);
	castsShadow = false;
	isShadowed = false;
	normalMapped = false;
}

void MeshActor::Render()
{
	glUseProgram(shaderProgram);
	FreeBuffers();
	SetUniforms();
	SetBufferData();
	DrawBuffers();
	CleanUp();
	FreeBuffers();
}

void MeshActor::ShadowPass()
{
	// Compute the MVP matrix from the light's point of view
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-25, 25, -25, 25, -25, 25);
	glm::mat4 depthViewMatrix = glm::lookAt(scene->light, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 depthModelMatrix = GetModelMatrix();
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	// compute depthbiasMVP for actual shading step later
	depthBiasMVP = biasMatrix*depthMVP;

	if (!castsShadow) return;	//hop off here if we don't want this to cast shadows

	glUseProgram(shadowProgram);

	// set MVP in shadow shader
	GLint depthMatrixID = glGetUniformLocation(shadowProgram, "depthMVP");
	if (depthMatrixID != -1)
	{
		glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);
	}

	FreeBuffers();

	glBindVertexArray(vao);

	// set element buffer with face indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		mesh.mFaces.size() * sizeof(glm::ivec3),
		mesh.mFaces.data(),
		GL_STATIC_DRAW
		);

	// set array buffer with vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		mesh.mVertices.size() * sizeof(glm::vec3),
		mesh.mVertices.data(),
		GL_STATIC_DRAW
		);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// draw by index
	glDrawElements(
		GL_TRIANGLES,
		mesh.mFaces.size() * 3,
		GL_UNSIGNED_INT,
		NULL
	);

	glDisableVertexAttribArray(0);

	FreeBuffers();
}

void MeshActor::GenBuffers()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &elementBuffer);
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &normalBuffer);
	glGenBuffers(1, &tangentBuffer);
	glGenBuffers(1, &bitangentBuffer);
}

void MeshActor::FreeBuffers()
{
	//unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshActor::SetUniforms()
{
	// attempt setting M, V, P, VP matrices in shader
	model = GetModelMatrix();
	GLint modelMatID = glGetUniformLocation(shaderProgram, "model");
	if (modelMatID != -1)
	{
		glUniformMatrix4fv(modelMatID, 1, GL_FALSE, glm::value_ptr(model));
	}

	GLint viewMatID = glGetUniformLocation(shaderProgram, "view");
	if (viewMatID != -1)
	{
		glUniformMatrix4fv(viewMatID, 1, GL_FALSE, glm::value_ptr(scene->camera->view));
	}

	GLint projMatID = glGetUniformLocation(shaderProgram, "projection");
	if (projMatID != -1)
	{
		glUniformMatrix4fv(projMatID, 1, GL_FALSE, glm::value_ptr(scene->camera->projection));
	}

	GLint VPMatID = glGetUniformLocation(shaderProgram, "view_projection");
	if (VPMatID != -1)
	{
		glUniformMatrix4fv(VPMatID, 1, GL_FALSE, glm::value_ptr(scene->camera->VP));
	}

	GLint lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos");
	if (lightPosLocation != -1)
	{
		glUniform3fv(lightPosLocation, 1, &scene->light[0]);
	}

	GLint viewPosLocation = glGetUniformLocation(shaderProgram, "viewPos");
	if (viewPosLocation != -1)
	{
		glUniform3fv(viewPosLocation, 1, &scene->camera->position[0]);
	}

	GLint useShadowLoc = glGetUniformLocation(shaderProgram, "useShadowMapping");
	if (useShadowLoc != -1)
	{
		if (isShadowed)
		{
			glUniform1i(useShadowLoc, 1);
			
		}
		else glUniform1i(useShadowLoc, 0);
	}

	GLint depthBiasLoc = glGetUniformLocation(shaderProgram, "depthBiasMVP");
	if (depthBiasLoc != -1)
	{
		glUniformMatrix4fv(depthBiasLoc, 1, GL_FALSE, glm::value_ptr(depthBiasMVP));
	}

	GLint depthTexLocation = glGetUniformLocation(shaderProgram, "shadowMap");
	if (depthTexLocation != -1)
	{
		glUniform1i(depthTexLocation, 10);
	}
}

void MeshActor::SetBufferData()
{
	glBindVertexArray(vao);

	if (isShadowed)
	{
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, scene->depthTexture);
	}

	// set element buffer with face indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		mesh.mFaces.size() * sizeof(glm::ivec3),
		mesh.mFaces.data(),
		GL_STATIC_DRAW
	);

	// set array buffer with vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		mesh.mVertices.size() * sizeof(glm::vec3),
		mesh.mVertices.data(),
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// normals
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		mesh.mNormals.size() * sizeof(glm::vec3),
		mesh.mNormals.data(),
		GL_STATIC_DRAW
		);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// tangents
	glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		mesh.mTangents.size() * sizeof(glm::vec3),
		mesh.mTangents.data(),
		GL_STATIC_DRAW
		);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// binormals
	glBindBuffer(GL_ARRAY_BUFFER, bitangentBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		mesh.mBitangents.size() * sizeof(glm::vec3),
		mesh.mBitangents.data(),
		GL_STATIC_DRAW
		);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void MeshActor::DrawBuffers()
{
	glDrawElements(
		GL_TRIANGLES,
		mesh.mFaces.size() * 3,
		GL_UNSIGNED_INT,
		NULL
	);
}

void MeshActor::CleanUp()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

std::string MeshActor::SetMesh(std::string & pFile)
{
	return aiu::Import(mesh, pFile);
}

void MeshActor::SetShader(const char* vertProgram, const char* fragProgram)
{
	shaderProgram = ShaderLoader::CompileVertFrag(vertProgram, fragProgram);
}

void MeshActor::SetShadowShader(const char * vertProgram, const char * fragProgram)
{
	shadowProgram = ShaderLoader::CompileVertFrag(vertProgram, fragProgram);
}
