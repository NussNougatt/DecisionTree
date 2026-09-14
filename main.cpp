#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <chrono>
#include "Node.hpp"
#include "DecisionTree.hpp"
#include <algorithm>

struct tSplit
{
  DataFrame train;
  DataFrame test;
};

DataFrame readCSV(const std::string& filename)
{
  DataFrame data;
  std::ifstream file(filename);

  if (!file.is_open())
  {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return data;
  }

  std::string line;
  while (std::getline(file, line))
  {
    if(line.empty()) continue;

    std::vector<float> row;
    std::stringstream ss(line);
    std::string cell;

    while (std::getline(ss, cell, ','))
    {
      row.push_back(((cell == "Iris-setosa" ? 0.0f : (cell == "Iris-versicolor") ? 1.0f : (cell == "Iris-virginica") ? 2.0f : std::stof(cell))));
    }

    data.push_back(row);
  }

  file.close();
  return data;
}

//For now just return an 80/20 Split
tSplit train_test_split(const DataFrame& data)
{
  tSplit result;
  std::vector<size_t> indices(data.size());
  for(size_t i = 0; i < indices.size(); i++) indices[i] = i;

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(indices.begin(), indices.end(), std::default_random_engine(seed));

  size_t test_size = static_cast<size_t>(data.size() * 0.2);

  for(size_t i = 0; i < data.size(); i++)
  {
    if(i < test_size)
    {
      result.test.push_back(data[indices[i]]);
    }
    else
    {
      result.train.push_back(data[indices[i]]);
    }
  }

  return result;
}

void print_tree(Node* node, int depth = 0)
{
  std::string indent(depth * 2, ' ');

  if(node->is_leaf)
  {
    std::cout << indent << "Leaf: predict " << node->value << std::endl;
    return;
  }
  std::cout << indent << "Feature " << node->feature_idx << " <= " << node->threshold << " (gain=" << node->information_gain << ")" << std::endl;
  print_tree(node->left, depth + 1);
  print_tree(node->right, depth + 1);
}

int main()
{
  DataFrame data = readCSV("bezdekIris.csv");
  tSplit train_test = train_test_split(data);

  DecisionTree dt(2, 2);
  dt.fit(train_test.train);
  std::vector<float> predictions = dt.predict(train_test.test);

  int counter = 0;

  for(int i = 0; i < predictions.size(); i++)
  {
    if(predictions.at(i) == train_test.test.at(i)[train_test.test[0].size() - 1])
    {
      counter += 1;
    }
  }

  float accuracy = static_cast<float>(counter) / predictions.size();
  std::cout << "Accuracy: " << accuracy << std::endl;
  
  print_tree(dt.root);

  return 0;
}
















