#include "DecisionTree.hpp"

#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <cmath>
#include <set>

DecisionTree::DecisionTree(int min_samples, int max_depth)
{
  this.min_samples = min_samples;
  this.max_depth = max_depth;
}

Node* DecisionTree::build_tree(DataFrame data, int label_column)
{
  std::vector<float> y = isolate(data, label_column);
  DataFrame X = without(data, y);

  depth = 0;
  samples = X.size();

  if(depth <= this.max_depth && samples >= this.min_samples)
  {
    bestSplit best_split = best_split(DataFrame data, int n_features);

    if(best_split.information_gain > 0)
    {
      Node left_child = build_tree(best_split.left, depth + 1);
      Node right_child = build_tree(best_split.right, depth + 1);

      return new Node(best_split.left, best_split.right, best_split.feature_idx, best_split.threshold, best_split.information_gain);
    }
  }
  
  int most_common = 0;
  std::unordered_map<float, int> freqs;
  for(const auto& n : y):
  {
    freqs[n]++;
    if(freqs[n] > freqs[most_common])
    {
      most_common = n;
    }
  }
  int value = most_common;
  return new Node(value);
}

//Implement sizeOfDataframe_y function
bestSplit DecisionTree::best_split(DataFrame data, int n_features)
{
  bestSplit best_split = new bestSplit;
  
  for(int i = 0; i < n_features; i++)
  {
    DataFrame feature_values = isolate(data, feature_idx);
    std::set<float> thresholds(feature_values.bgein(), feature_values.end());
    int thresholds_len = thresholds.size();

    for(int j = 0; j < thresholds_len; j++)
    {
      DataFrames split = split(data, feature_idx, thresholds[i]);

      if(split.left != NULL && split.right != NULL)
      {
        std::vector<float> parent_labels = isolate(data, sizeOfDataframe_y(data));
        std::vector<float> left_labels = isolate(split.left, sizeOfDataframe_y(split.left));
        std::vector<float> right_labels = isolate(split.right, sizeOfDataframe_y(split.right));

        float information_gain = DecisionTree::information_gain(parent_labels, left_labels, right_labels);

        if(information_gain > best_split.information_gain)
        {
          best_split.information_gain = information_gain;
          best_split.threshold = thresholds[j];
          best_split.feature_idx = i;
          best_split.left = split.left;
          best_split.right = split.right;
        }
      }
    }
  
    return best_split;
}

DataFrames DecisionTree::split(const DataFrame& data, size_t feature_idx, float thresholds)
{
  DataFrames split;
  
  DataFrame left = new DataFrame;
  DataFrame right = new DataFrame;

  for(size_t i = 0; i < data.size(); i++)
  {
    if(data[i][feature_idx] <= threshold)
    {
      result.left.push_back(data[i]);
    }
    else 
    {
      result.right.push_back(data[i]);  
    }
 }
  
  return split;
}

float DecisionTree::information_gain(std::vector<int> parent_labels, std::vector<int> left_labels, std::vector<int> right_labels)
{
  float left_weight = static_cast<float>(left_labels.size()) / parent_labels.size();
  float right_weight = static_cast<float>(right_labels.size()) / parent_labels.size();

  float information_gain = DecisionTree::entropy(parent_labels) - (left_weight * DecisionTree::entropy(left_labels) + right_weight * DecisionTree::entropy(right_labels));

  return information_gain;
}

float DecisionTree::entropy(std::vector<int> y)
{
  float entropy = 0;
  std::set<int> class_labels(y.begin(), y.end());
  
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

void DecisionTree::fit(DataFrame X, DataFrame y)
{
  DataFrame data;
  data.reserve(X.size());

  for(size_t i = 0; i < X.size(); i++)
  {
    std::vector<float> row = X[i];
    row.push_back(y[i]);
    data.push_back(row);
  }

  root = build_tree(data, static_cast<int>(data.size()));
}

std::vector<int> DecisionTree::predict(DataFrame X, Node root)
{
  std::vector<int> predictions;
  predictions.reserve(X.size());

  for(const auto& row : X)
  {
    predictions.push_back(predict_class(row, root));
  }

  return predictions;
}

int DecisionTree::predict_class(const std::vector<int>& row, Node* node)
{
  if(node ->.has_value())
  {
    return node->value.value();
  }

  int feature_value = row[node->feature_idx];
  if(feature_value <= node->threshold)
  {
    return predict_class(row, node->left);
  }
  else
  {
    return predict_class(row, node->right);
  }
}

DataFrame DecisionTree::without_last(DataFrame data)
{
  for(const auto& row : data)
  {
    row.pop_back();
  }

  return data;
}

std::vector<float> DecisionTree::isolate(DataFrame data)
{
  std::vector<float> column;

  for(const auto& row : data)
  {
    column.push_back(row[row.size() - 1]);
  }

  return column;
}














