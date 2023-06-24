#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <queue>
#include <stack>
#include "graph.h"
using namespace std;

void error(string msg)
{
    cout << "Error: " << msg << endl;
}

DisjointSet::DisjointSet(int numVertices)
{
    subsets.resize(numVertices);
    for (unsigned int i = 0; i < subsets.size(); ++i)
        DisjointSet::makeSet(i);
}

void DisjointSet::makeSet(Vertex x)
{
    subsets[x].parent = x;
}

Vertex DisjointSet::findSet(Vertex x)
{
    Vertex & parent = subsets[x].parent;
    if (parent != x)
        parent = findSet(parent);
    return parent;
}

void DisjointSet::unionSets(Vertex x, Vertex y)
{
    Vertex xroot = DisjointSet::findSet(x);
    Vertex yroot = DisjointSet::findSet(y);
    subsets[yroot].parent = xroot;
}

Graph Graph::sort_edges() const
{
    Graph copyG = *this;
    sort(copyG.begin(), copyG.end(), [](Edge const &a, Edge const &b) {
        return a.weight < b.weight; });
    return copyG;
}

VertexList Graph::edges_from(Vertex vertex) const
{
    VertexList edges;
    
    for (const Edge & edge : *this) {
        if (edge.u == vertex)
            edges.push_back(edge.v);
    }
    
    return edges;
}

EdgeList Kruskals(const Graph& G)
{
    DisjointSet ds{G.numVertices};
    const Graph& sorted_edges = G.sort_edges();
    size_t solution_size = G.numVertices - 1;
    EdgeList result;
    
    for (Edge e : sorted_edges) {
        Vertex uRoot = ds.findSet(e.u);
        Vertex vRoot = ds.findSet(e.v);
        
        if (uRoot != vRoot) {
            ds.unionSets(uRoot, vRoot);
            result.push_back(e);
            if (result.size() >= solution_size) break;
        }
    }
    return result;
}

int sum_weights(EdgeList const& L)
{
    unsigned int edgeListSize = L.size();
    unsigned int i;
    int weights = 0;
    for(i = 0; i < edgeListSize; ++i)
        weights += L[i].weight;
    return weights;
}

void file_to_graph(string filename, Graph & G)
{
    ifstream file(filename);
    file >> G;
    file.close();
}

VertexList dfs(const Graph& graph, Vertex startVertex)
{
    VertexList result;
    vector<bool> visited(graph.size(), false);
    stack<Vertex> stk;
    visited[startVertex] = true;
    stk.push(startVertex);
    while (!stk.empty()) {
        Vertex vertex = stk.top();
        stk.pop();
        result.push_back(vertex);
        for (Vertex v : graph.edges_from(vertex)) {
            if (!visited[v]) {
                visited[v] = true;
                stk.push(v);
            }
        }
    }
    return result;
}

VertexList bfs(const Graph& graph, Vertex startVertex)
{
    VertexList result;
    vector<bool> visited(graph.size(), false);
    queue<Vertex> queue;
    visited[startVertex] = true;
    queue.push(startVertex);
    
    while (!queue.empty()) {
        Vertex vertex = queue.front();
        queue.pop();
        result.push_back(vertex);
        for (Vertex v : graph.edges_from(vertex)) {
            if (!visited[v]) {
                visited[v] = true;
                queue.push(v);
            }
        }
    }
    return result;
}

string get_arg(int argc, char *argv[], string def)
{
    if (argc == 2)
        return string(argv[1]);
    else
        return def;
}