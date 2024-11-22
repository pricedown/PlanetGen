#pragma once
#include "mesh.h"

namespace pl {
	void createSphere(float radius, int subDivisions, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
}
