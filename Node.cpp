#include "Node.hpp"

Node::Node(Node* left, Node* right, int feature_idx, float threshold, float information_gain)
{
  this->left = left;
  this->right = right;
  this->feature_idx = feature_idx;
  this->threshold = threshold;
  this->information_gain = information_gain;

  this->value = -1;
  this->is_leaf = false;
}

Node::Node(float value)
{
  this->value = value;
  this->is_leaf = true;
  this->left = nullptr;
  this->right = nullptr;
}

Node::~Node()
{
  delete left;
  delete right;
}

bestSplit::bestSplit()
{
  this->information_gain = -std::numeric_limits<float>::infinity();
  this->threshold = 0.0f;
  this->feature_idx = -1;
}
