// Copyright 2019 P G
#include <iostream>
#include <map>
#include <set>
#include <vector>

using Graph = std::map<int, std::set<int>>;
using Edge = std::map<std::pair<int, int>, int>;

int Cmin = 1000;

void addEdge(Graph &g, int i, int j) { g[i].insert(j); }

void addEdge2(Graph &g, int i, int j) {
  addEdge(g, i, j);
  addEdge(g, j, i);
}

void f(int u, int v, int k, Edge &flow) {
  flow[{u, v}] = k;
  flow[{v, u}] = -k;
}

void c(int u, int v, int k, Edge &cop) { cop[{u, v}] = k; }

void dfs(Graph g, int v, int s, int t, Edge &flow, Edge &cop,
         std::map<int, bool> &visited) {
  if (v == t) {
    return;
  }
  visited[v] = true;
  for (auto &u : g[v]) {
    if (flow[{v, u}] <= 0) {
      if (not visited[u] and flow[{v, u}] > cop[{v, u}] and
          cop[{u, v}] > flow[{u, v}]) {
        Cmin = std::min(Cmin, std::min(flow[{v, u}] - cop[{v, u}],
                                       flow[{v, u}] + cop[{u, v}]));
        dfs(g, u, s, t, flow, cop, visited);
        flow[{v, u}] -= Cmin;
        flow[{u, v}] += Cmin;
      }
    }
    if (flow[{v, u}] >= 0) {
      if (not visited[u] and flow[{v, u}] < cop[{v, u}] and
          cop[{u, v}] < flow[{u, v}]) {
        Cmin = std::min(Cmin, std::min(flow[{u, v}] - cop[{u, v}],
                                       flow[{u, v}] + cop[{v, u}]));
        dfs(g, u, s, t, flow, cop, visited);
        flow[{v, u}] += Cmin;
        flow[{u, v}] -= Cmin;
      }
    }
    if (v == s) {
      Cmin = 1000;
    }
  }
  return;
}

Graph testGraph() {
  Graph g;
  addEdge2(g, 1, 2);
  addEdge2(g, 1, 3);
  addEdge2(g, 2, 4);
  addEdge2(g, 3, 4);
  return g;
}

int main() {
  std::cout << "Program shows max_flow for each pair of vertexes" << std::endl
            << "It runs in built-in test, which can be changed from inside"
            << std::endl;
  Graph g = testGraph();
  int s = 1;
  int t = 4;
  Edge flow;
  Edge cop;

  f(1, 2, 0, flow);
  f(1, 3, 0, flow);
  f(2, 4, 0, flow);
  f(3, 4, 0, flow);
  c(1, 2, 1, cop);
  c(1, 3, 3, cop);
  c(2, 4, 1, cop);
  c(3, 4, 2, cop);
  c(2, 1, -1, cop);
  c(3, 1, -2, cop);
  c(4, 2, -1, cop);
  c(4, 3, -2, cop);

  std::map<int, bool> visited;
  for (auto &i : g) {
    visited[i.first] = false;
  }
  dfs(g, s, s, t, flow, cop, visited);

  for (auto &i : flow) {
    std::cout << '(' << i.first.first << ',' << i.first.second << ") -> "
              << i.second << std::endl;
  }

  std::cout << "Was a vertex visited or not:" << std::endl;
  for (auto &i : visited) {
    std::cout << i.first << " ";
    if (i.second) {
      std::cout << "YES" << std::endl;
    } else {
      std::cout << "NO" << std::endl;
    }
  }
}
