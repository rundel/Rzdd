#ifndef TDD_GRAPH_UTIL_HPP
#define TDD_GRAPH_UTIL_HPP

#include <Rcpp.h>
#include <tdzdd/util/Graph.hpp>

inline tdzdd::Graph tdzdd_graph_from_adj(std::vector<std::vector<unsigned int>> const& adj_list,
                                        std::vector<std::string> labels = std::vector<std::string>()) {
  tdzdd::Graph g;
  
  if (labels.size() == 0) {
    for(size_t i=0; i != adj_list.size(); ++i) {
      labels.push_back(std::to_string(i+1));
    }
  }
  
  for(size_t i=0; i != adj_list.size(); ++i) {
    for(size_t j=0; j != adj_list[i].size(); ++j) {
      g.addEdge(labels[i], labels[adj_list[i][j]-1]);    
    }
  }
  g.update();
  
  return g;
}

inline tdzdd::Graph tzdd_graph_from_edgelist(Rcpp::DataFrame const& edges) {
  tdzdd::Graph g;
  
  if (edges.size() != 2)
    throw std::runtime_error("ERROR: expected two columns.");
  
  std::vector<std::string> from = edges[0];
  std::vector<std::string> to   = edges[1];
  
  for(size_t i=0; i != from.size(); ++i) {
    g.addEdge(from[i], to[i]);   
  }
  g.update();
  
  return g;
}

inline Rcpp::DataFrame tzdd_graph_to_edgelist(tdzdd::Graph const& g) {
  std::vector<std::string> from, to;
  
  for (int a = 0; a < g.edgeSize(); ++a) {
    auto e = g.edgeName(a);
    
    from.push_back(e.first);
    to.push_back(e.second);
  }
  
  return Rcpp::DataFrame::create(
    Rcpp::Named("from") = from,
    Rcpp::Named("to") = to,
    Rcpp::Named("stringsAsFactors") = false
  );
}

#endif