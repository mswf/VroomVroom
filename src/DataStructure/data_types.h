#ifndef data_types_h
#define data_types_h

#include <vector>
#include "../glm/vec2.hpp"
#include "../glm/vec3.hpp"

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
	//unsigned int normalBuffer;
	//unsigned int vertexBuffer;
	//unsigned int uvBuffer;
	//unsigned int tangentBuffer;
	//unsigned int bitangentBuffer;
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
