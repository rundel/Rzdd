#ifndef ZDD_TREE_HPP
#define ZDD_TREE_HPP

// [[Rcpp::plugins(cpp11)]]

struct zdd_tree {
  std::vector<int> layer;
  std::vector<int> arc0;
  std::vector<int> arc1;
  
  void add_node(int i, int a0, int a1) {
    layer.push_back(i);
    arc0.push_back(a0);
    arc1.push_back(a1);
  }
  
  Rcpp::DataFrame as_df() {
    std::vector<size_t> layer_start;
    std::vector<int> layer_R;
    std::vector<std::string> label;
    
    for(size_t i=1; i < layer.size(); ++i) {
      if (layer[i] != layer[i-1]) {
        layer_start.push_back(i);
      }
    }
    
    for(size_t i=0, j=0; i < layer.size(); ++i) {
      if (i == layer_start[j])
        ++j;
      
      if      (arc0[i] == -2) arc0[i] = NA_INTEGER;
      else if (arc0[i] == -1) arc0[i] = layer.size()+1;
      else                    arc0[i] += layer_start[j]+1;
      
      if      (arc1[i] == -2) arc1[i] = NA_INTEGER;
      else if (arc1[i] == -1) arc1[i] = layer.size()+1;
      else                    arc1[i] += layer_start[j]+1;
      
      layer_R.push_back(layer[i]+1);
      label.push_back("E[" + std::to_string(layer[i]+1) + "]");
    }
    
    layer_R.push_back(layer_start.size()+2);
    arc0.push_back(NA_INTEGER);
    arc1.push_back(NA_INTEGER);
    label.push_back("T");
    
    
    return Rcpp::DataFrame::create(
      Rcpp::Named("label") = label,
      Rcpp::Named("layer") = layer_R,
      Rcpp::Named("arc0") = arc0,
      Rcpp::Named("arc1") = arc1
    );
  }
};

#endif