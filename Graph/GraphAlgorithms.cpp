#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>

struct MST
{
	std::vector<std::tuple<size_t, size_t, int>> edges;
	size_t sumOfWeights;
};

class UnionFind
{
public:

	UnionFind(int n) : parent(n), height(n, 0)
	{
		for (int i = 0; i < n; ++i)
			parent[i] = i;
	}

	int getRoot(int n)
	{
		if (parent[n] != n)
			parent[n] = getRoot(parent[n]);
		return parent[n];
	}
	bool areInOneSet(int n, int k)
	{
		return getRoot(n) == getRoot(k);
	}
	bool Union(int n, int k)
	{
		int root1 = getRoot(n);
		int root2 = getRoot(k);

		if (root1 == root2)
		{
			return false;
		}
		if (height[root1] < height[root2])
		{
			parent[root1] = root2;
		}
		else if (height[root1] > height[root2])
		{
			parent[root2] = root1;
		}
		else
		{
			parent[root2] = root1;
			height[root1]++;
		}

		return true;
	}

private:

	std::vector<int> parent;
	std::vector<int> height;
};

using Edge = std::tuple<size_t, size_t, int>;

class Graph
{
public:

	Graph(size_t V, bool isOriented) : oriented(isOriented), V(V), adj(V) {}

	void addEdge(size_t start, size_t end, int weight)
	{
		adj[start].push_back({ end, weight });
		if (!oriented)
			adj[end].push_back({ start, weight });
	}

	size_t dijstra(size_t start, size_t end, std::vector<size_t>& path) const
	{
		struct VertexDistance
		{
			size_t vertex;
			size_t distFromStart;

			bool operator<(const VertexDistance& other) const
			{
				return distFromStart > other.distFromStart;
			}
		};

		std::vector<size_t> distances(V, INT_MAX);
		std::vector<size_t> prevs(V);
		std::priority_queue<VertexDistance> q;
		distances[start] = 0;
		q.push({start, 0});

		while (!q.empty())
		{
			auto current = q.top();
			q.pop();

			if (current.vertex == end)
			{
				while (end != start)
				{
					path.push_back(end);
					end = prevs[end];
				}

				path.push_back(start);
				std::reverse(path.begin(), path.end());

				return distances[current.vertex];
			}

			for (int i = 0; i < adj[current.vertex].size(); ++i)
			{
				size_t currentNeighbor = adj[current.vertex][i].first;

				size_t newDist = current.distFromStart + adj[current.vertex][i].second;
				if (newDist < distances[currentNeighbor])
				{
					distances[currentNeighbor] = newDist;
					q.push({ currentNeighbor, newDist });
					prevs[currentNeighbor] = current.vertex;
				}
			}
		}

		return INT_MAX;
	}

	MST Prim() const
	{
		struct primEdge
		{
			size_t start;
			size_t end;
			int weight;

			bool operator<(const primEdge& other) const
			{
				return weight > other.weight;
			}
		};

		MST result;
		result.sumOfWeights = 0;
		std::priority_queue<primEdge> q;
		std::vector<bool> visited(V);
		q.push({0, 0, 0});
		size_t addedEdges = 0;
		bool isFirst = true;

		while (addedEdges < V && !q.empty())
		{
			auto current = q.top();
			q.pop();

			if (visited[current.end])
			{
				continue;
			}

			visited[current.end] = true;
			for (int i = 0; i < adj[current.end].size(); ++i)
			{
				q.push({ current.end, (size_t)adj[current.end][i].first, adj[current.end][i].second });
			}
			
			if (isFirst)
			{
				isFirst = false;
				continue;
			}

			result.edges.push_back({ current.start, current.end, current.weight });
			result.sumOfWeights += current.weight;
			addedEdges++;
		}

		return result;
	}

	MST Kruskal() const
	{
		MST result;
		result.sumOfWeights = 0;
		std::vector<Edge> edges;

		for (int i = 0; i < V; ++i)
		{
			for (auto& p : adj[i])
			{
				edges.push_back({ i, (size_t)p.first, p.second });
			}
		}

		std::sort(edges.begin(), edges.end(), [](const Edge& lhs, const Edge& rhs) {
			return std::get<2>(lhs) < std::get<2>(rhs);
			});

		UnionFind uf(V);
		size_t edgesIncluded = 0;

		for (int i = 0; i < edges.size() && edgesIncluded < V - 1; ++i)
		{
			auto& e = edges[i];
			if (uf.Union(std::get<0>(e), std::get<1>(e)))
			{
				result.edges.push_back(e);
				result.sumOfWeights += std::get<2>(e);
				edgesIncluded++;
			}
		}

		return result;
	}

private:

	bool oriented;
	size_t V;
	std::vector<std::vector<std::pair<int, int>>> adj;

};
