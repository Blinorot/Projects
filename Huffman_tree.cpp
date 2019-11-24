// Copyright 2019 P G
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using byte = unsigned char;

struct Node {
  unsigned freq;
  unsigned left;
  unsigned right;
};

using Tree = std::vector<Node>;

Tree Huff(const std::vector<byte> &bytes) {
  std::vector<unsigned> freqs(256, 0);

  for (auto &x : bytes) {
    freqs[x]++;
  }

  Tree huff;
  for (unsigned i = 0; i < 256; i++) {
    Node node;
    node.left = -1;
    node.right = -1;
    node.freq = freqs[i];

    huff.push_back(node);
  }

  std::vector<unsigned> roots;
  for (unsigned i = 0; i < 256; i++) {
    roots.push_back(i);
  }

  while (roots.size() > 1) {
    sort(roots.begin(), roots.end(), [&](unsigned i, unsigned j) {
      // Ð¾Ð¿Ð¸ÑÑ‹Ð²Ð°Ð¹ Ð·Ð´ÐµÑÑŒ Ñ„ÑƒÐ½ÐºÑ†Ð¸ÑŽ "Ñ‡Ð°ÑÑ‚Ð¾Ñ‚Ð° i-Ð³Ð¾
      // Ð´ÐµÑ€ÐµÐ²Ð° _Ð±Ð¾Ð»ÑŒÑˆÐµ_ Ñ‡Ð°ÑÑ‚Ð¾Ñ‚Ñ‹ j-Ð³Ð¾"
      return huff[i].freq > huff[j].freq;
    });

    auto tmp = roots.back();
    roots.pop_back();
    Node node;
    node.freq = huff[tmp].freq + huff[roots.back()].freq;
    node.left = tmp;
    node.right = roots.back();
    huff.push_back(node);
    roots.pop_back();
    roots.push_back(huff.size() - 1);
  }

  return huff;
}

std::set<unsigned> neighbours(const Tree &Hoff, unsigned i) {
  std::set<unsigned> a{};
  if (Hoff[i].left != (unsigned)(-1)) {
    a.insert(Hoff[i].left);
  }
  if (Hoff[i].right != (unsigned)(-1)) {
    a.insert(Hoff[i].right);
  }
  return a;
}

std::vector<std::vector<unsigned>> Path(const Tree &Hoff) {
  std::vector<std::vector<unsigned>> Full_path(256, std::vector<unsigned>());
  std::set<unsigned> component(const Tree &Hoff, int v);
  std::vector<unsigned> path(1, Hoff.size() - 1);
  std::set<unsigned> visited{};

  while (path.size() > 0) {
    unsigned current_vertex = path.back();
    visited.insert(current_vertex);

    bool found_new = false;

    auto n = neighbours(Hoff, current_vertex);
    for (unsigned i : n) {
      if (visited.count(i) == 0) {
        path.push_back(i);
        found_new = true;
        break;
      }
    }

    if (found_new) {
      continue;
    }
    if (0 <= path.back() && path.back() < 256) {
      Full_path[path.back()] = path;
    }
    path.pop_back();
  }

  return Full_path;
}

std::ostream &operator<<(std::ostream &os, Node n) {
  os << "{" << n.freq << " | " << n.left << ", " << n.right << "}";

  return os;
}

int main() {
  std::vector<byte> bytes;
  std::cout << "Answer will be a tree based on input text" << std::endl
            << "Format: number of symb -> branch from root to this number"
            << std::endl;
  byte b;
  while (true) {
    b = std::cin.get();
    if (!std::cin) {
      break;
    }
    bytes.push_back(b);
  }
  Tree huff = Huff(bytes);
  std::vector<std::vector<unsigned>> path = Path(huff);

  // for (auto x: huff){
  //   cout << i++ << " -> " << x << endl;
  // }
  std::cout << path.size();
  for (unsigned x = 0; x < path.size(); x++) {
    std::cout << x << " -> (";
    for (auto &y : path[x]) {
      std::cout << y << ", ";
    }
    std::cout << ")" << std::endl;
  }
}
