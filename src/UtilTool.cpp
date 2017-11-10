#include "UtilTool.h"

using namespace std;
using namespace Eigen;

double random() {
	return 1.0 * rand() / RAND_MAX;
}

void UtilTool::distributeBorderVertex(const Vector2d& center, const double length, const double bias, const int borderVertexNum, vector<Vertex>& vertexSet) {
	Vertex vertex;
	vertex.type = BORDER;
	// 4 vertex
	vertex.pos[0] = center(0) - length / 2, vertex.pos[1] = center(1) - length / 2;
	vertexSet.push_back(vertex);
	vertex.pos[0] = center(0) - length / 2, vertex.pos[1] = center(1) + length / 2;
	vertexSet.push_back(vertex);
	vertex.pos[0] = center(0) + length / 2, vertex.pos[1] = center(1) - length / 2;
	vertexSet.push_back(vertex);
	vertex.pos[0] = center(0) + length / 2, vertex.pos[1] = center(1) + length / 2;
	vertexSet.push_back(vertex);
	// other border point
	int divideNum = (borderVertexNum - 4) / 4;
	for (int i = 1; i <= divideNum; i++) {
		vertex.pos[0] = center(0) - length / 2 - bias * sin(1.0*i / divideNum * PI);		// left edge
		vertex.pos[1] = center(1) - length / 2 + i * length / (divideNum + 1);
		vertexSet.push_back(vertex);
		vertex.pos[0] = center(0) + length / 2 + bias * sin(1.0*i / divideNum * PI);		// right edge
		vertex.pos[1] = center(1) - length / 2 + i * length / (divideNum + 1);
		vertexSet.push_back(vertex);
		vertex.pos[0] = center(0) - length / 2 + i * length / (divideNum + 1);				// bottom edge
		vertex.pos[1] = center(1) - length / 2 - bias * sin(1.0*i / divideNum * PI);
		vertexSet.push_back(vertex);
		vertex.pos[0] = center(0) - length / 2 + i * length / (divideNum + 1);				// top edge
		vertex.pos[1] = center(1) + length / 2 + bias * sin(1.0*i / divideNum * PI);
		vertexSet.push_back(vertex);
	}
}

void UtilTool::distributeInnerVertex(const Vector2d& center, const double length, const int innerVertexNum, vector<Vertex>& vertexSet) {
	Vertex vertex;
	vertex.type = INNER;
	for (int i = 0; i < innerVertexNum; i++) {
		vertex.pos[0] = center(0) - length / 2 + 0.99 * length * random(), vertex.pos[1] = center(1) - length / 2 + 0.99 * length * random();
		vertexSet.push_back(vertex);
	}
}

void UtilTool::generateSuperTri(const Vector2d& center, const double length, vector<Vertex>& superTri) {
	Vertex v1, v2, v3;
	v1.pos[0] = center(0),				v1.pos[1] = center(1) + 1.5 * length;
	v2.pos[0] = center(0) - 2 * length, v2.pos[1] = center(1) - length;
	v3.pos[0] = center(0) + 2 * length, v3.pos[1] = center(1) - length;
	superTri.push_back(v1);
	superTri.push_back(v2);
	superTri.push_back(v3);
}

bool compare(Vertex a, Vertex b) {
	if (a.pos[0] < b.pos[0]) return true;
	if (a.pos[0] == b.pos[0] && a.pos[1] < b.pos[1]) return true;
	return false;
}
void UtilTool::sortVertexSet(vector<Vertex>& vertexSet) {
	sort(vertexSet.begin(), vertexSet.end(), compare);
}

void UtilTool::generateVertexMat(vector<Vertex>& vertexSet_vec, MatrixX2d& vertexSet_mat) {
	int vertexNum = vertexSet_vec.size();
	for (int i = 0; i < vertexNum; i++) {
		for (int j = 0; j < 2; j++) {
			vertexSet_mat(i,j) = vertexSet_vec[i].pos[j];
		}
	}
}

void UtilTool::generateVertexType(vector<Vertex>& vertexSet_vec, vector<int>& vertexType) {
	vertexType.clear();
	for (int i = 0; i < vertexSet_vec.size(); i++) {
		vertexType.push_back(vertexSet_vec[i].type);
	}
}

void UtilTool::backToVertexVec(MatrixX2d& vertexSet_mat, vector<Vertex>& vertexSet_vec) {
	for (int i = 0; i < vertexSet_mat.rows(); i++) {
		for (int j = 0; j < 2; j++) {
			vertexSet_vec[i].pos[j] = vertexSet_mat(i, j);
		}
	}
}