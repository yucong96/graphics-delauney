#include "head.h"
#include "UtilTool.h"
#include "TriTool.h"
#include "HomoTool.h"
#include "DeformTool.h"

Eigen::Vector2d center;
double length = 1;
double bias = 0.01*length;
double radius = 0.8;
int borderVertexNum = 40;
int innerVertexNum = 60;

int homoNum = 10;
int triNum = 5;
int deformNum = 20;

std::vector<Vertex> vertexSet_vec;
std::vector<Face> faceSet;
std::vector<Color> colorSet;
std::vector<Vertex> superTri;
Eigen::MatrixX2d vertexSet_mat(borderVertexNum + innerVertexNum, 2);
std::vector<int> vertexType;

bool begin = true;
int t = 0;
int tfreq = 70;

UtilTool utilTool;
HomoTool homoTool;
TriTool triTool;
DeformTool deformTool;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);	
	for (int i = 0; i < faceSet.size(); i++) {
		if (begin) {
			Color c(1.0 * rand() / RAND_MAX, 1.0 * rand() / RAND_MAX, 1.0 * rand() / RAND_MAX);
			colorSet.push_back(c);
		}
		glColor3f(colorSet[i].color[0], colorSet[i].color[1], colorSet[i].color[2]);
		glBegin(GL_TRIANGLES); {
			glVertex2d(vertexSet_vec[faceSet[i].vertex[0]].pos[0], vertexSet_vec[faceSet[i].vertex[0]].pos[1]);
			glVertex2d(vertexSet_vec[faceSet[i].vertex[1]].pos[0], vertexSet_vec[faceSet[i].vertex[1]].pos[1]);
			glVertex2d(vertexSet_vec[faceSet[i].vertex[2]].pos[0], vertexSet_vec[faceSet[i].vertex[2]].pos[1]);
		}
		glEnd();
	}
	begin = false;
	glutSwapBuffers();
}

void idle(void) {
	if (t % tfreq == 0) {
		if (t >= deformNum * 2 * tfreq) {
			t = 0;
		}
		if ((t / tfreq) % (deformNum * 2) < deformNum) {
			deformTool.next(vertexSet_mat);
		}
		else {
			deformTool.prev(vertexSet_mat);
		}
		homoTool.run(vertexSet_mat, vertexType, faceSet, homoNum, HomoTool::NOT_REFRESH);
		utilTool.backToVertexVec(vertexSet_mat, vertexSet_vec);
	}
	t++;
	display();
}

void init() {
	srand((unsigned)time(NULL));
	center << 0, 0;

	utilTool.distributeBorderVertex(center, length, bias, borderVertexNum, vertexSet_vec);
	utilTool.distributeInnerVertex(center, length, innerVertexNum, vertexSet_vec);
	utilTool.generateSuperTri(center, length, superTri);
	utilTool.sortVertexSet(vertexSet_vec);
	utilTool.generateVertexMat(vertexSet_vec, vertexSet_mat);
	utilTool.generateVertexType(vertexSet_vec, vertexType);

	for (int i = 0; i < triNum; i++) {
		triTool.run(superTri, vertexSet_mat, faceSet, TriTool::SORTED);
		homoTool.run(vertexSet_mat, vertexType, faceSet, homoNum, HomoTool::REFRESH);
		utilTool.backToVertexVec(vertexSet_mat, vertexSet_vec);
		if (i != triNum - 1) {
			utilTool.sortVertexSet(vertexSet_vec);
			utilTool.generateVertexMat(vertexSet_vec, vertexSet_mat);
			utilTool.generateVertexType(vertexSet_vec, vertexType);
		}
	}

	deformTool.start(vertexSet_mat, vertexType, center, length, radius, borderVertexNum, deformNum);
}

int main(int argc, char** argv) {
	init();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Graphics_1");
	glutDisplayFunc(&display);
	glutIdleFunc(&idle);
	glutMainLoop();
	return 0;
}