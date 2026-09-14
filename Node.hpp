#ifndef NODE_HPP
#define NODE_HPP

#include <vector>

using DataFrame = std::vector<std::vector<float>>;

struct bestSplit
{
  float information_gain;
  DataFrame left;
  DataFrame right;
  float threshold;
  int feature_idx;

  bestSplit();
};

struct Node
{
  Node* left;
  Node* right;
  int feature_idx;
  float threshold;
  int information_gain;
  bool is_leaf;

  float value;

  Node(Node& left, Node& right, int feature_idx, float threshold, int information_gain);

  Node(float value);

  ~Node();

};

#endif
