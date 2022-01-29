#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846f
#endif

struct Float3
{
	float x;
	float y;
	float z;
};

struct Float4
{
	float x;
	float y;
	float z;
	float w;
};

struct Vertex
{
	Float3 Position;
	Float4 Color;
};

void MatrixRotationRollPitchYaw(float* matrix, float pitch, float yaw, float roll)
{

}