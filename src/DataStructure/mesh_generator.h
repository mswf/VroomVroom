#pragma once

#include "data_types.h"

class ModelInstance;

void Cube( ModelInstance*& instance );
void Quad( ModelInstance*& instance );
void GenerateCircle( std::vector< glm::vec3 >& points, float radius, int segments );
void EnvironmentCube( ModelInstance*& instance );