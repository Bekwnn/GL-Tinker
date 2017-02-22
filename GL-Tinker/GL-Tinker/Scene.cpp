#include "Scene.h"

#include <assimp\scene.h>
#include <assimp\postprocess.h>

Scene::Scene()
{
	light = glm::vec3(0.025f, 1.f, 0.f);
	glGenTextures(1, &depthTexture);
	glGenFramebuffers(1, &shadowFrameBuffer);
}

void Scene::UpdateScene()
{
	for (auto& actor : sceneActors)
	{
		actor->Tick();
	}

	camera->Tick();

	//set and bind frame buffer for shadow render
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBuffer);
	glClear(GL_DEPTH_BUFFER_BIT);

	//set depth texture for shadowmapping
	glActiveTexture(GL_TEXTURE10); // SHADOW MAP IS IN TEXTURE 10, DO NOT USE
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	//glDrawBuffer(GL_NONE); // don't draw to color buffer

	// check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("blah");

	glViewport(0, 0, 2048, 2048); //opengl renders at viewport resolution
	for (auto& actor : sceneActors)
	{
		actor->ShadowPass();
	}
	glViewport(0, 0, 1280, 720); //change back to old viewport resolution

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	for (auto& actor : sceneActors)
	{
		actor->Render();
	}
}