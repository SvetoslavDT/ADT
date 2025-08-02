#pragma once
#include <vector>

class UnionFind
{
public:

	UnionFind(int n);
	int getRoot(int n);
	bool areInOneSet(int n, int k);
	bool Union(int n, int k);

private:

	std::vector<int> parent;
	std::vector<int> height;
};
