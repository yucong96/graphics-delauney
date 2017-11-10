#pragma once

#include "head.h"

class UtilTool {
public:
	void distributeBorderVertex(const Eigen::Vector2d& center, const double length, const double bias, const int borderVertexNum, std::vector<Vertex>& vertexSet);
	void distributeInnerVertex(const Eigen::Vector2d& center, const double length, const int innerVertexNum, std::vector<Vertex>& vertexSet);
	void generateSuperTri(const Eigen::Vector2d& center, const double length, std::vector<Vertex>& superTri);
	void sortVertexSet(std::vector<Vertex>& vertexSet);
	void generateVertexMat(std::vector<Vertex>& vertexSet_vec, Eigen::MatrixX2d& vertexSet_mat);
	void generateVertexType(std::vector<Vertex>& vertexSet_vec, std::vector<int>& vertexType);
	void backToVertexVec(Eigen::MatrixX2d& vertexSet_mat, std::vector<Vertex>& vertexSet_vec);
};