#pragma once

#include "head.h"
#include "Queue.h"

class TriTool
{
public:
	static const int NOT_SORT = 0;
	static const int SORTED = 1;
	void run(std::vector<Vertex>& superTri, Eigen::MatrixX2d& vertexSet, std::vector<Face>& faceSet, int isSorted);
};

