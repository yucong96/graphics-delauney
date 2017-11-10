#include "DeformTool.h"

using namespace std;
using namespace Eigen;

bool doubleCompare(double a, double b, double tolerance) {
	if (a > b - tolerance && a < b + tolerance) {
		return true;
	}
	return false;
}

void DeformTool::start(MatrixX2d& vertexSet, vector<int>& vertexType, Vector2d& center, double length, double radius, int borderVertexNum, int _steps) {
	int vertexNum = vertexType.size();
	double pieceLength = length / (borderVertexNum / 4);

	deformMat = MatrixX2d::Zero(vertexNum, 2);
	MatrixX2d destMat(vertexNum, 2);
	destMat = MatrixX2d::Zero(vertexNum, 2);

	for (int i = 0; i < vertexNum; i++) {
		if (vertexType[i] == BORDER) {
			int startIndex, biasIndex;
			if (doubleCompare(vertexSet(i, 0) - center(0), length / 2, pieceLength / 2)) {
				// right border
				startIndex = borderVertexNum / 8 * 7;
				biasIndex = (vertexSet(i, 1) - center(1) + length / 2 + pieceLength / 2) / pieceLength;
			}
			else if (doubleCompare(vertexSet(i, 0) - center(0), -length / 2, pieceLength / 2)) {
				// left border
				startIndex = borderVertexNum / 8 * 3;
				biasIndex = (center(1) + length / 2 - vertexSet(i, 1) + pieceLength / 2) / pieceLength;
			}
			else if (doubleCompare(vertexSet(i, 1) - center(1), length / 2, pieceLength / 2)) {
				// top border
				startIndex = borderVertexNum / 8 * 1;
				biasIndex = (center(0) + length / 2 - vertexSet(i, 0) + pieceLength / 2) / pieceLength;
			}
			else {
				// bottom border
				startIndex = borderVertexNum / 8 * 5;
				biasIndex = (vertexSet(i, 0) - center(0) + length / 2 + pieceLength / 2) / pieceLength;
			}
			double theta = ((startIndex + biasIndex) % borderVertexNum) * (2 * PI / borderVertexNum);
			destMat(i, 0) = center(0) + radius * cos(theta), destMat(i, 1) = center(1) + radius * sin(theta);
		}
		else {
			destMat(i, 0) = vertexSet(i, 0);
			destMat(i, 1) = vertexSet(i, 1);
		}
	}
	deformMat = destMat - vertexSet;
	deformMat /= _steps;
	steps = _steps;
	nowStep = 0;
}

void DeformTool::next(MatrixX2d& vertexSet) {
	if (nowStep < steps) {
		vertexSet += deformMat;
		nowStep++;
	}
}

void DeformTool::prev(MatrixX2d& vertexSet) {
	if (nowStep > 0) {
		vertexSet -= deformMat;
		nowStep--;
	}
}