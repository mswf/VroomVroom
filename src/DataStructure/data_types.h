#ifndef data_types_h
#define data_types_h

#include <vector>
#include <map>
#include <string>
#include "../glm/vec2.hpp"
#include "../glm/vec3.hpp"

struct ShaderProgram;

struct ShaderObject
{
	std::map< std::string, unsigned int > locations;
	unsigned int shader = 0;
	unsigned int shaderType = 0;
	// Create by the number of subroutine uniforms & clean up when finished
	//TODO(Valentinas): Use std::Array instead of pointer to memory block
	unsigned int* subroutines;
	unsigned int numSubroutines = 0;
	bool updateProgram = true;
	ShaderProgram* program;
};

struct ShaderProgram
{
	unsigned int program = 0;
	std::string name = "No name";
	// Create by the number of shader objects & clean up when finished
	std::vector< ShaderObject* > shaders;
	bool separate = false;
};

struct Mesh
{
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
	std::vector< glm::vec3 > bitangents;
	std::vector< glm::vec3 > tangents;
	std::vector< unsigned int > indices;
	unsigned int numIndices = 0;
	unsigned int materialId = 0;
	float scaleFactor = 1;
	bool isBuffered = false;
	bool hasBufferChanged = false;
	bool hasPositions = false;
	bool hasNormals = false;
	bool hasTangentsAndBitangets = false;
	bool hasUVs = false;
};

struct ModelInstance
{
	unsigned int vao = 0;
	unsigned int vbo = 0;
	unsigned int numIndices = 0;
	unsigned int indiceBuffer = 0;
	unsigned int materialId = 0;
};

struct ImageData
{
	int imageId = -1;
	unsigned char* pixelData;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int components = 0;
	bool isBuffered = false;
	bool mipmapping = false;
};

#endif /* data_types_h */
