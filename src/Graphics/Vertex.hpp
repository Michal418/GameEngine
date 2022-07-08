#pragma once

struct Vertex
{
	float x, y;
	float tu, tv;
};

struct Triangle
{
	Vertex a, b, c;
};

struct Quad
{
	Triangle a, b;
};