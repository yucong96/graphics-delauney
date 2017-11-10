#include "HomoTool.h"

using namespace std;
using namespace Eigen;

void HomoTool::run(MatrixX2d& vertexSet, vector<int>& vertexType, vector<Face>& faceSet, int iterNum, int option) {
	if (option == REFRESH) {
		generateMatrix(vertexType, faceSet);
	}
	for (int i = 0; i < iterNum; i++) {
		vertexSet = homoMat * vertexSet;
	}
}

void HomoTool::generateMatrix(vector<int>& vertexType, vector<Face>& faceSet) {
	int vertexNum = vertexType.size();
	homoMat = SparseMatrix<double, RowMajor>(vertexNum, vertexNum);
	homoMat.reserve(7 * vertexNum);
	homoMat.setZero();
	SparseMatrix<double> coeff(vertexNum, vertexNum);
	coeff.reserve(vertexNum);
	coeff.setZero();

	for (int i = 0; i < faceSet.size(); i++) {
		for (int j = 0; j < 3; j++) {
			if (vertexType[faceSet[i].vertex[j]] == INNER) {
				int next = (j + 1) % 3, nnext = (j + 2) % 3;
				homoMat.coeffRef(faceSet[i].vertex[j], faceSet[i].vertex[next]) += 1;
				homoMat.coeffRef(faceSet[i].vertex[j], faceSet[i].vertex[nnext]) += 1;
				coeff.coeffRef(faceSet[i].vertex[j], faceSet[i].vertex[j]) += 2;
			}
		}
	}

	for (int i = 0; i < vertexNum; i++) {
		if (vertexType[i] == BORDER) {
			homoMat.coeffRef(i, i) = 1;
			coeff.coeffRef(i, i) = 1;
		}
		else {
			coeff.coeffRef(i, i) = 1.0 / coeff.coeffRef(i, i);
		}
	}

	homoMat = coeff * homoMat;
}