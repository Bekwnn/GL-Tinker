#include "AssimpUtil.h"

#include <glm\gtc\type_ptr.hpp>
#include "stb_image.h"

using namespace aiu;

Mesh::Mesh()
{
	mVertices = std::vector<glm::vec3>();
	mUVCoords = std::vector<glm::vec2>();
	mFaces = std::vector<glm::ivec3>();
}

Mesh::Mesh(aiMesh* aimesh)
{
	//PER VERT
	int vertCount = aimesh->mNumVertices;
	mVertices = std::vector<glm::vec3>(vertCount);
	mNormals = std::vector<glm::vec3>(vertCount);
	mTangents = std::vector<glm::vec3>(vertCount);
	mBitangents = std::vector<glm::vec3>(vertCount);
	mUVCoords = std::vector<glm::vec2>(vertCount);
	for (int vertIdx = 0; vertIdx < vertCount; vertIdx++)
	{
		mVertices[vertIdx] = glm::make_vec3(&aimesh->mVertices[vertIdx][0]);

		if (aimesh->HasNormals())
		{
			mNormals[vertIdx] = glm::make_vec3(&aimesh->mNormals[vertIdx][0]);
		}

		// Check first uv mapping channel (0) for texture coordinates
		if (aimesh->HasTextureCoords(0))
		{
			mUVCoords[vertIdx] = glm::make_vec2(&aimesh->mTextureCoords[0][vertIdx][0]);
		}

		if (aimesh->HasTangentsAndBitangents())
		{
			mTangents[vertIdx] = glm::make_vec3(&aimesh->mTangents[vertIdx][0]);
			mBitangents[vertIdx] = glm::make_vec3(&aimesh->mBitangents[vertIdx][0]);
		}
	}

	//PER FACE
	int faceCount = aimesh->mNumFaces;
	mFaces = std::vector<glm::ivec3>(faceCount);
	for (int faceIdx = 0; faceIdx < faceCount; faceIdx++)
	{
		mFaces[faceIdx] = glm::make_vec3(aimesh->mFaces[faceIdx].mIndices);
	}
}

Texture::Texture()
{ }

Texture::Texture(const char* ifile)
{
	data = stbi_load(ifile, &x, &y, &n, 4); //forces 4 8 bit components for rgba
}

Texture::~Texture()
{
	stbi_image_free(data);
}

std::string aiu::Import(aiu::Mesh& mesh, std::string& pfile)
{
	// Create an instance of the Importer class
	Assimp::Importer importer;
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll 
	// propably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(pfile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	// If the import failed, report it
	if (!scene)
	{
		return importer.GetErrorString();
	}

	// Now we can access the file's contents.
	// TODO: access the file's meshes and return them
	aiNode* curNode = scene->mRootNode;
	while (curNode->mNumMeshes == 0)
	{
		if (curNode->mChildren == NULL)
			break;
		else
			curNode = curNode->mChildren[0];
	}
	if (curNode->mNumMeshes != 0)
	{
		mesh = aiu::Mesh(scene->mMeshes[curNode->mMeshes[0]]);
	}

	// We're done. Everything will be cleaned up by the importer destructor
	return "FBX Scene imported successfully. Mesh: " + std::to_string(mesh.mVertices.size());
}