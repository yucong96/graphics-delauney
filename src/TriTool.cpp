#include "TriTool.h"

using namespace std;
using namespace Eigen;




void addSuperTri(const vector<Vertex>& superTri, MatrixX2d& vertexSet, Queue<Face>& tempTri) {
	vertexSet.conservativeResize(vertexSet.rows() + 3, vertexSet.cols());
	Vertex superVertex;			// push super vertices into vertex set
	for (int i = 0; i < 3; i++) {
		superVertex = superTri[i];
		vertexSet(vertexSet.rows() + i - 3, 0) = superVertex.pos[0];
		vertexSet(vertexSet.rows() + i - 3, 1) = superVertex.pos[1];
	}
	Face superTriIndex(vertexSet.rows() - 3, vertexSet.rows() - 2, vertexSet.rows() - 1);	// push super triangular into tempTri set
	tempTri.push(superTriIndex);
}

void deleteSuperTri(Queue<Face>& validTri, int vertexNum, vector<Face>& faceSet, MatrixX2d& vertexSet) {
	while (validTri.length() > 0) {
		Face tri = validTri.pop();
		if (tri.vertex[2] >= vertexNum) continue;
		faceSet.push_back(tri);
	}
	vertexSet.conservativeResize(vertexSet.rows() - 3, vertexSet.cols());
}

const int IN_CIRCLE = 1;
const int OUT_NOT_RIGHT_CIRCLE = 2;
const int OUT_RIGHT_CIRCLE = 3;
int inTriCircle(MatrixX2d& vertexSet, Face& tri, int vertexIndex) {
	double x1, y1, x2, y2, x3, y3, xt, yt, x0, y0;
	double a, b, c, d, e, f;
	double r2;

	// compute the center
	x1 = vertexSet(tri.vertex[0], 0), y1 = vertexSet(tri.vertex[0], 1);
	x2 = vertexSet(tri.vertex[1], 0), y2 = vertexSet(tri.vertex[1], 1);
	x3 = vertexSet(tri.vertex[2], 0), y3 = vertexSet(tri.vertex[2], 1);
	xt = vertexSet(vertexIndex, 0), yt = vertexSet(vertexIndex, 1);
	a = x1 - x2;
	b = y1 - y2;
	c = x1 - x3;
	d = y1 - y3;
	e = ((x1*x1 - x2*x2) - (y2*y2 - y1*y1)) / 2;
	f = ((x1*x1 - x3*x3) - (y3*y3 - y1*y1)) / 2;
	x0 = -(d*e - b*f) / (b*c - a*d);
	y0 = -(a*f - c*e) / (b*c - a*d);
	r2 = (x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0);

	if (((xt - x0)*(xt - x0) + (yt - y0)*(yt - y0)) < r2) {
		return IN_CIRCLE;
	}
	else if ((xt - x0)*(xt - x0) < r2) {
		return OUT_NOT_RIGHT_CIRCLE;
	}
	else {
		return OUT_RIGHT_CIRCLE;
	}
}

bool faceIsEqual(const Face& a, const Face& b) {
	if (a.vertex[0] == b.vertex[0] && a.vertex[1] == b.vertex[1] && a.vertex[2] == b.vertex[2]) return true;
	return false;
}
bool edgeIsEqual(const Edge& a, const Edge& b) {
	if (a.vertex[0] == b.vertex[0] && a.vertex[1] == b.vertex[1]) return true;
	return false;
}
void TriTool::run(vector<Vertex>& superTri, MatrixX2d& vertexSet, vector<Face>& faceSet, int isSorted) {
	faceSet.clear();

	int vertexNum = vertexSet.rows();
	Queue<Face> validTri(faceIsEqual);
	Queue<Face> tempTri(faceIsEqual);
	Queue<Face> faceBuf(faceIsEqual);
	Queue<Edge> edgeBuf(edgeIsEqual);
	Queue<Edge> repeatEdge(edgeIsEqual);

	if (NOT_SORT) {
		cout << "Please sort outside the TriTool" << endl;
		return;
	}

	addSuperTri(superTri, vertexSet, tempTri);
	for (int i = 0; i < vertexNum; i++) {
		int validTriNum = validTri.length();
		int tempTriNum = tempTri.length();
		for (int j = 0; j < tempTriNum; j++) {
			Face tri = tempTri.pop();
			Vector2d point = vertexSet.row(i);
			int state = inTriCircle(vertexSet, tri, i);
			switch (state) {
			case IN_CIRCLE: {
				Edge edge1(tri.vertex[0], tri.vertex[1]);
				Edge edge2(tri.vertex[0], tri.vertex[2]);
				Edge edge3(tri.vertex[1], tri.vertex[2]);
				if (edgeBuf.push(edge1) == Queue<Edge>::IN_QUEUE) {
					repeatEdge.push(edge1);
				}
				if (edgeBuf.push(edge2) == Queue<Edge>::IN_QUEUE) {
					repeatEdge.push(edge2);
				}
				if (edgeBuf.push(edge3) == Queue<Edge>::IN_QUEUE) {
					repeatEdge.push(edge3);
				}

				Face newTri1, newTri2, newTri3;
				if (i < tri.vertex[0]) {
					newTri1.set(i, tri.vertex[0], tri.vertex[1]);
					newTri2.set(i, tri.vertex[0], tri.vertex[2]);
					newTri3.set(i, tri.vertex[1], tri.vertex[2]);
				}
				else if (i < tri.vertex[1]) {
					newTri1.set(tri.vertex[0], i, tri.vertex[1]);
					newTri2.set(tri.vertex[0], i, tri.vertex[2]);
					newTri3.set(i, tri.vertex[1], tri.vertex[2]);
				}
				else if (i < tri.vertex[2]) {
					newTri1.set(tri.vertex[0], tri.vertex[1], i);
					newTri2.set(tri.vertex[0], i, tri.vertex[2]);
					newTri3.set(tri.vertex[1], i, tri.vertex[2]);
				}
				else {
					newTri1.set(tri.vertex[0], tri.vertex[1], i);
					newTri2.set(tri.vertex[0], tri.vertex[2], i);
					newTri3.set(tri.vertex[1], tri.vertex[2], i);
				}
				faceBuf.push(newTri1);
				faceBuf.push(newTri2);
				faceBuf.push(newTri3);
				
				break;
			}
			case OUT_NOT_RIGHT_CIRCLE: {
				tempTri.push(tri);
				break;
			}
			case OUT_RIGHT_CIRCLE: {
				validTri.push(tri);
				break;
			}
			}
		}
		while (faceBuf.length() > 0) {
			Face tri = faceBuf.pop();
			bool deleteFlag = false;
			for (int i = 0; i < repeatEdge.length(); i++) {
				if ((tri.vertex[0] == repeatEdge.at(i).vertex[0] && tri.vertex[1] == repeatEdge.at(i).vertex[1]) ||
					(tri.vertex[0] == repeatEdge.at(i).vertex[0] && tri.vertex[2] == repeatEdge.at(i).vertex[1]) ||
					(tri.vertex[1] == repeatEdge.at(i).vertex[0] && tri.vertex[2] == repeatEdge.at(i).vertex[1])) {
					deleteFlag = true;
					break;
				}
			}
			if (!deleteFlag) {
				tempTri.push(tri);
			}
		}
		faceBuf.clear();
		edgeBuf.clear();
		repeatEdge.clear();
		/*for (int i = 0; i < validTri.length(); i++) {
			Face tri = validTri.at(i);
			int t = 0;
		}
		for (int i = 0; i < tempTri.length(); i++) {
			Face tri = tempTri.at(i);
			int t = 0;
		}*/
	}
	// Combine
	while (tempTri.length() != 0) {
		validTri.push(tempTri.pop());
	}
	// Delete SuperTri
	deleteSuperTri(validTri, vertexNum, faceSet, vertexSet);
	
}
