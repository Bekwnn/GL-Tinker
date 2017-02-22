// utility namespace for assimp functions

#pragma once
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>

#include <vector>
#include <glm\glm.hpp>

namespace aiu
{
	// class for copying mesh data to
	class Mesh
	{
	public:
		Mesh();
		Mesh(aiMesh* aimesh);

		std::vector<glm::vec3> mVertices;
		std::vector<glm::vec3> mNormals;
		std::vector<glm::vec3> mTangents;
		std::vector<glm::vec3> mBitangents;
		std::vector<glm::ivec3> mFaces;
		std::vector<glm::vec2> mUVCoords;
	};

	class Texture
	{
	public:
		Texture();
		Texture(const char* ifile);
		~Texture();

		int x, y, n;
		unsigned char* data;
	};

	// returns string describing success, and mesh as out parameter
	std::string Import(Mesh& mesh, std::string& pfile);
}