#include <Rcpp.h>
#include "Rzdd.h"
#include "tdzdd/util/Graph.hpp"
#include "graph.hpp"

//' @param adj_list Adjacency list
//' @export
// [[Rcpp::export]]
void test_graph(std::vector<std::vector<unsigned int>> adj_list) {

  tdzdd::Graph g;
  
  for(size_t i=0; i != adj_list.size(); ++i) {
    for(size_t j=0; j != adj_list[i].size(); ++j) {
      g.addEdge(std::to_string(i), std::to_string(adj_list[i][j]-1));    
    }
  }
  
  g.update();
  
  Rcpp::Rcout << "Updated:\n";
  Rcpp::Rcout << g << "\n";
  
  
  graph g2(adj_list, true);
  g2.print_edge_list();
}


