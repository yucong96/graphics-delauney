#pragma once

#include "head.h"

class DeformTool {
private:
	int steps, nowStep;
	Eigen::MatrixX2d deformMat;
public:
	void start(Eigen::MatrixX2d& vertexSet, std::vector<int>& vertexType, Eigen::Vector2d& center, double length, double radius, int borderVertexNum, int steps);
	void next(Eigen::MatrixX2d& vertexSet);
	void prev(Eigen::MatrixX2d& vertexSet);
};