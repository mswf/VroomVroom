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
	std::vector< unsigned int > indices;
	unsigned int numIndices;
	bool hasNormals;
	bool hasUVs;
};




#endif /* data_types_h */
