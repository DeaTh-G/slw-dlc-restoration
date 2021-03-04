#pragma once

struct alignas(16) Matrix34
{
	float data[4][4];
};

__declspec(align(16)) struct Vector3
{
	float X;
	float Y;
	float Z;

	Vector3()
	{
		X = 0;
		Y = 0;
		Z = 0;
	}

	Vector3(float x, float y, float z)
	{
		X = x;
		Y = y;
		Z = z;
	}
};