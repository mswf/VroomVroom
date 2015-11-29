#ifndef data_types_h
#define data_types_h

#include <vector>
#include <map>
#include "../glm/vec2.hpp"
#include "../glm/vec3.hpp"

struct ShaderObject
{
	unsigned int shader;
	unsigned int shaderType;
	// Create by the number of subroutine uniforms & clean up when finished
	unsigned int* subroutines;
	unsigned int numSubroutines;
	bool updateProgram;
	std::map< std::string, unsigned int > locations;
};

struct ShaderProgram
{
	unsigned int program;
	// Create by the number of shader objects & clean up when finished
	std::vector< ShaderObject* > shaders;
	bool separate;
};

struct Mesh
{
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
	std::vector< glm::vec3 > bitangents;
	std::vector< glm::vec3 > tangents;
	std::vector< unsigned int > indices;
	unsigned int numIndices;
	unsigned int materialId;
	bool isBuffered;
	bool hasBufferChanged;
	bool hasPositions;
	bool hasNormals;
	bool hasTangentsAndBitangets;
	bool hasUVs;
	Mesh():
		numIndices(0),
		materialId(0),
		isBuffered(false),
		hasBufferChanged(false),
		hasPositions(false),
		hasNormals(false),
		hasTangentsAndBitangets(false),
		hasUVs(false)
 	{}
};

struct ModelInstance
{
	unsigned int vao;
	unsigned int vbo;
	unsigned int numIndices;
	unsigned int indiceBuffer;
	unsigned int materialId;
	ModelInstance():
		vao(0),
		vbo(0),
		numIndices(0),
		indiceBuffer(0),
		materialId(0)
	{}
};






#endif /* data_types_h */
