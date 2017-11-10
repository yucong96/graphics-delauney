#pragma once

#include "head.h"

class HomoTool {
private:
	Eigen::SparseMatrix<double> homoMat;
public:
	static const int NOT_REFRESH = 0;
	static const int REFRESH = 1;
	void run(Eigen::MatrixX2d& vertexSet, std::vector<int>& vertexType, std::vector<Face>& faceSet, int iterTime, int option);
private:
	void generateMatrix(std::vector<int>& vertexType, std::vector<Face>& faceSet);
};