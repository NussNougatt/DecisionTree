#include "DecisionTree.hpp"

#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <cmath>
#include <set>

DecisionTree::DecisionTree(int min_samples, int max_depth)
{
  this->min_samples = min_samples;
  this->max_depth = max_depth;
  this->root = nullptr;
}

Node* DecisionTree::build_tree(DataFrame data, int label_column, int depth)
{
  std::vector<float> y = isolate_column(data, label_column);
  int n_features = static_cast<int>(data[0].size()) - 1;
  int samples = static_cast<int>(data.size());

  if(depth <= max_depth && samples >= min_samples)
  {
    bestSplit best = find_best_split(data, label_column, n_features);

    if(best.information_gain > 0)
    {
      Node* left_child = build_tree(best.left, label_column, depth + 1);
      Node* right_child = build_tree(best.right, label_column, depth + 1);

      return new Node(left_child, right_child, best.feature_idx, best.threshold, best.information_gain);
    }
  }
  
  float leaf_value = most_common(y);
  return new Node(leaf_value);
}

bestSplit DecisionTree::find_best_split(DataFrame data, int label_column, int n_features)
{
  bestSplit best;

  std::vector<float> parent_labels = isolate_column(data, label_column);
  
  for(int feature_idx = 0; feature_idx < n_features; feature_idx++)
  {
    std::vector<float> feature_values = isolate_column(data, feature_idx);
    std::set<float> thresholds(feature_values.begin(), feature_values.end());

    for(float threshold : thresholds)
    {
      DataFrames candidate = split(data, feature_idx, threshold);

      if(!candidate.left.empty() && !candidate.right.empty())
      {
        std::vector<float> left_labels = isolate_column(candidate.left, label_column);
        std::vector<float> right_labels = isolate_column(candidate.right, label_column);

        float gain = information_gain(parent_labels, left_labels, right_labels);

        if(gain > best.information_gain)
        {
          best.information_gain = gain;
          best.threshold = threshold;
          best.feature_idx = feature_idx;
          best.left = candidate.left;
          best.right = candidate.right;
        }
      }
    }
  }
  
  return best;
}

DataFrames DecisionTree::split(const DataFrame& data, size_t feature_idx, float threshold)
{
  DataFrames output;

  for(size_t i = 0; i < data.size(); i++)
  {
    if(data[i][feature_idx] <= threshold)
    {
      output.left.push_back(data[i]);
    }
    else 
    {
      output.right.push_back(data[i]);
    }
 }
  
  return output;
}

float DecisionTree::information_gain(const std::vector<float>& parent_labels, 
                                      const std::vector<float>& left_labels, 
                                      const std::vector<float>& right_labels)
{
  float left_weight = static_cast<float>(left_labels.size()) / parent_labels.size();
  float right_weight = static_cast<float>(right_labels.size()) / parent_labels.size();

  float information_gain = DecisionTree::entropy(parent_labels) - (left_weight * DecisionTree::entropy(left_labels) + right_weight * DecisionTree::entropy(right_labels));

  return information_gain;
}

float DecisionTree::entropy(const std::vector<float>& y)
{
  if(y.empty()) return 0.0f;

  float entropy = 0.0f;
  std::set<float> class_labels(y.begin(), y.end());
  
  for(const auto& class_label : class_labels)
  {
    int count = 0;
    for(const auto& label : y)
    {
      if(label == class_label) count++;
    }

    float p = static_cast<float>(count) / y.size();
    entropy += -p * std::log2(p);
  }

  return entropy;
}

void DecisionTree::fit(DataFrame X)
{
  int label_column = static_cast<int>(X[0].size()) - 1;
  root = build_tree(X, label_column, 0);
}

std::vector<float> DecisionTree::predict(const DataFrame& X)
{
  std::vector<float> predictions;
  predictions.reserve(X.size());

  for(const auto& row : X)
  {
    predictions.push_back(predict_class(row, root));
  }

  return predictions;
}

float DecisionTree::predict_class(const std::vector<float>& row, Node* node)
{
  if(node->is_leaf)
  {
    return node->value;
  }

  float feature_value = row[node->feature_idx];
  if(feature_value <= node->threshold)
  {
    return predict_class(row, node->left);
  }
  else
  {
    return predict_class(row, node->right);
  }
}

DataFrame DecisionTree::drop_column(const DataFrame& data, size_t col_idx)
{
  DataFrame result = data;

  for(auto& row : result)
  {
    row.erase(row.begin() + col_idx);
  }

  return result;
}

std::vector<float> DecisionTree::isolate_column(const DataFrame& data, size_t col_idx)
{
  std::vector<float> column;
  column.reserve(data.size());

  for(const auto& row : data)
  {
    column.push_back(row[col_idx]);
  }

  return column;
}


float DecisionTree::most_common(const std::vector<float>& y)
{
  std::unordered_map<float, int> freqs;
  float best_label = y[0];
  int best_count = 0;

  for(float label : y)
  {
    int count = ++freqs[label];
    if(count > best_count)
    {
      best_count = count;
      best_label = label;
    }
  }

  return best_label;
}

DecisionTree::~DecisionTree()
{
  delete root;
}














