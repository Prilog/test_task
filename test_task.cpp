#pragma once

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#include "list_lib.h"
#include "svn.h"

using namespace std;

string makeLine(string arg) {
	string result = arg;
	result += "\n";
	for (int i = 0; i < arg.size(); i++) {
		result += "-";
	}
	result += "\n";
	return result;
}

bool TestList(vector<pair<string, int>> &arg) {
	List test = List(arg);
	FILE* stream;
	fopen_s(&stream, "output.txt", "wb");
	test.Serialize(stream);
	fclose(stream);
	fopen_s(&stream, "output.txt", "rb");
	List result = List();
	result.Deserialize(stream);
	fclose(stream);
	return test.IsEqual(result);
}

void TestSerialization() {
	cout << makeLine("Testing Serialization and Deserialization");
	vector<pair<string, int>> v;
	string str = "";
	bool allDone = true;
	for (int i = 0; i < 100; i++) {
		v.push_back(make_pair(str, rand() % (i + 1)));
		str += char(rand() % 256);
		bool res = TestList(v);
		allDone &= res;
		cout << "Test " << i << " : " << (res ? "Done" : "Failed") << "\n";
	}
	cout << (allDone ? makeLine("All correct") : makeLine("Mistake"));
}

void SmallTestSmoothNormals() {
	cout << makeLine("Testing Smooth Normals");
	vec3 normals[8];
	vec3 vertex[4] = { vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 1, 0), vec3(0, 0, 1) };
	int faces[12] = { 0, 2, 3, 0, 1, 3, 0, 1, 2, 1, 2, 3 };
	calc_mesh_normals(normals, vertex, faces, 4, 4);
	for (int i = 0; i < 4; i++) {
		cout << normals[i] << " ";
	}
	cout << "\n" << makeLine("Finished");
}

int main()
{
	TestSerialization();
	SmallTestSmoothNormals();
}
