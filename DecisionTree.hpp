#ifndef DECISIONTREE_HPP
#define DECISIONTREE_HPP

#include "Node.hpp"

struct DataFrames
{
  DataFrame left;
  DataFrame right;
};

class DecisionTree
{
  public:
    Node* root;
    const int min_samples;
    const int max_depth;

    DecisionTree(int min_samples, int max_depth);

    ~DecisionTree();

    Node* build_tree(DataFrame data, int label_column);

    bestSplit best_split(DataFrame data, int n_features);

    DataFrames split(DataFrame data, int feature_idx, float threshold);

    float information_gain(DataFrame parent_labels, DataFram left_labels, DataFrame right_labels);

    float entropy(DataFrame labels);

    void fit(DataFrame X, DataFrame y);

    float[] predict(DataFrame X);
};


#endif
