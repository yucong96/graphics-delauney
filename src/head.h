#pragma once

#define FREEGLUT_STATIC

#include <gl/glut.h>

#include <Eigen/Sparse>
#include <Eigen/Geometry>

#include <iostream>
#include <vector>
#include <time.h>

const int BORDER = 1;
const int INNER = 2;
const int SUPERTRI = 3;

const double PI = 3.1415926;

struct Vertex {
public:
	double pos[2];
	int type;
};

struct Face {
public:
	int vertex[3];
	Face() {}
	Face(int a, int b, int c) {
		set(a, b, c);
	}
	void set(int a, int b, int c) {
		vertex[0] = a;
		vertex[1] = b;
		vertex[2] = c;
	}
};

struct Edge {
public:
	int vertex[2];
	Edge() {}
	Edge(int a, int b) {
		set(a, b);
	}
	void set(int a, int b) {
		vertex[0] = a;
		vertex[1] = b;
	}
};

struct Color {
public:
	double color[3];
	Color() {}
	Color(double a, double b, double c) {
		set(a, b, c);
	}
	void set(double a, double b, double c) {
		color[0] = a;
		color[1] = b;
		color[2] = c;
	}
};