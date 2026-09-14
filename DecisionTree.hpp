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
    int min_samples;
    int max_depth;

    DecisionTree(int min_samples, int max_depth);

    ~DecisionTree();

    Node* build_tree(DataFrame data, int label_column, int depth);
    bestSplit best_split(DataFrame data, int label_column, int n_features);
    DataFrames split(const DataFrame& data, size_t feature_idx, float threshold);
    float information_gain(const std::vector<float>& parent_labels, 
                            const std::vector<float>& left_labels, 
                            const std::vector<float>& right_labels);
    float entropy(const std::vector<float>& y);
    void fit(DataFrame X, std::vector<float> y);
    std::vector<float> predict(const DataFrame& X);
    float predict_class(const std::vector<float>& row, Node* node);

    std::vector<float> isolate_column(const DataFrame& data, size_t col_idx);
    DataFrame drop_column(const DataFrame& data, size_t col_idx);
    float most_common(const std::vector<float>& y);
};


#endif
