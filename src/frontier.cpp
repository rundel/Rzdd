#include <Rcpp.h>

#include "graph.hpp"
#include "frontier.hpp"
#include "util.hpp"

// [[Rcpp::export]]
std::vector<std::vector<int>> compute_frontier(std::vector<std::vector<unsigned int>> adj_list) {
  
  graph g(adj_list, true); // Adjust to 0-based indexing
  frontier f(g);
  
  return f.as_list();
}


