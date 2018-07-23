#ifndef UTIL_HPP
#define UTIL_HPP

#include <Rcpp.h>

// [[Rcpp::plugins(cpp11)]]

class graph;

std::vector<std::vector<int> > to_0_indexing(std::vector<std::vector<int> > adj) {
  for(size_t i=0; i<adj.size(); i++) {
    for(size_t j=0; j<adj[i].size(); j++) {
      adj[i][j]--;
    }
  }  
  
  return adj;
}

std::vector<std::vector<int> > to_1_indexing(std::vector<std::vector<int> > adj) {
  for(size_t i=0; i<adj.size(); i++) {
    for(size_t j=0; j<adj[i].size(); j++) {
      adj[i][j]++;
    }
  } 
  
  return adj;
}

#endif