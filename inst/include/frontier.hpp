#ifndef FRONTIER_HPP
#define FRONTIER_HPP

// [[Rcpp::plugins(cpp11)]]

#include "graph.hpp"

class frontier {
private:
  std::vector<std::set<int> > F;
  
public:
  
  frontier(graph const& g) {
    F.resize(g.get_n_edges() + 1);
    F[0] = std::set<int>();
    
    for (size_t i = 0; i < g.get_n_edges(); ++i) {
      F[i + 1] = std::set<int>(F[i]);
      
      edge e = g.get_edge(i);
      int src = e.first;
      int dest = e.second;
      
      F[i + 1].insert(src);
      F[i + 1].insert(dest);
      
      if (!g.find_in_edge_list(src, i+1))
        F[i+1].erase(src);
      
      if (!g.find_in_edge_list(dest, i+1))
        F[i+1].erase(dest);
    }
  }
  
  void print() const {
    for(size_t i=0; i < F.size(); ++i) {
      Rcpp::Rcout << "F[" << i << "]: "
                  << to_string(i) << "\n";
    }
    Rcpp::Rcout << "\n";
  }
  
  
  std::string to_string(int i) const {
    std::string s;
    for(auto f : F[i])
      s += std::to_string(f) + " ";

    if (F[i].size() != 0)
      s += "\b";
    
    return s;
  }

  bool contains(int i, int value) const {
    return F[i].find(value) != F[i].end();
  }
  

  
  int size(int i) const {
    return F[i].size();
  }

  std::vector<std::vector<int>> as_list() const {
    std::vector<std::vector<int>> res;
    for(auto const& f : F) {
      std::vector<int> v(f.begin(), f.end());
      res.push_back(v);
    }
    
    return res;
  }
    
  std::vector<std::set<int>> const& get_frontier() const {
    return F;
  }
  
  std::set<int> const& get_frontier(int i) const {
    return F[i];
  }
  
  int get_frontier(int i, int j) const {
    return *std::next(F[i].begin(), j);
  }
};

#endif