#pragma once

#include <iostream>
#include <vector>
#include <cmath>

const double EPS = 1e-9;

struct vec3 {
	double x, y, z;

	vec3(double a = 0, double b = 0, double c = 0) : x(a), y(b), z(c) {}

	vec3 operator+(const vec3& arg) const {
		return vec3(x + arg.x, y + arg.y, z + arg.z);
	}

	vec3& operator+=(const vec3& arg) {
		x += arg.x;
		y += arg.y;
		z += arg.z;
		return *this;
	}

	vec3 operator-(const vec3& arg) const {
		return vec3(x - arg.x, y - arg.y, z - arg.z);
	}

	vec3 operator*(const double& arg) const {
		return vec3(x * arg, y * arg, z * arg);
	}

	vec3 Cross(const vec3& arg) const {
		return vec3(y * arg.z - z * arg.y, z * arg.x - x * arg.z, x * arg.y - y * arg.x);
	}

	double Dot(const vec3& arg) const {
		return x * arg.x + y * arg.y + z * arg.z;
	}

	double Magnitude() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	double Angle(const vec3& arg) const {
		return std::acos(Dot(arg)/(Magnitude() * arg.Magnitude()));
	}

	void Normalize() {
		double len = Magnitude();
		if (len < EPS) {
			return;
		}
		x /= len;
		y /= len;
		z /= len;
	}
};

std::ostream& operator<<(std::ostream& os, const vec3& arg) {
	os << "[" << arg.x << ' ' << arg.y << ' ' << arg.z << "]";
	return os;
}

//
// Calculate smooth (average) per-vertex normals
//
// [out] normals - output per-vertex normals
// [in] verts - input per-vertex positions
// [in] faces - triangles (triplets of vertex indices)
// [in] nverts - total number of vertices (# of elements in verts and normals arrays)
// [in] nfaces - total number of faces (# of elements in faces array)
//
//Note: style this function's name differs, because i were following style from previous task
//

void calc_mesh_normals(vec3* normals, const vec3* verts, const int* faces, int nverts, int nfaces) {
	//It is hard to store nfaces triangles in an array with nfaces size, so i presume, that its size is 3 * nfaces
	for (int i = 0; i < nfaces; i++) {
		vec3 a = verts[faces[3 * i]];
		vec3 b = verts[faces[3 * i + 1]];
		vec3 c = verts[faces[3 * i + 2]];
		
		vec3 n = (b - a).Cross(c - a);
		double ang0 = (b - a).Angle(c - a);
		double ang1 = (c - b).Angle(a - b);
		double ang2 = (a - c).Angle(b - c);

		normals[faces[3 * i]] += n * ang0;
		normals[faces[3 * i + 1]] += n * ang1;
		normals[faces[3 * i + 2]] += n * ang2;
	}
	for (int i = 0; i < nverts; i++) {
		normals[i].Normalize();
	}
}