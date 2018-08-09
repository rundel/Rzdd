#include <Rcpp.h>
#include "Rzdd.h"

#include <tdzdd_graph_util.hpp>
#include <tdzdd/util/Graph.hpp>


// [[Rcpp::export]]
int graph_frontier_max_size(std::vector<std::vector<unsigned int>> adj) {
  tdzdd::Graph g = tdzdd_graph_from_adj(adj);
  
  return g.maxFrontierSize();
}
