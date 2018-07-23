#include <Rcpp.h>
#include "Rzdd.h"
#include "tdzdd/DdStructure.hpp"


// [[Rcpp::export]]
std::string dd_to_dot(dd_ptr dd) {
  std::stringstream ss;
  dd->dumpDot(ss, "dd"); 
  return ss.str();
}


// [[Rcpp::export]]
std::string dd_to_sapporo(dd_ptr dd) {
  std::stringstream ss;
  dd->dumpSapporo(ss); 
  return ss.str();
}


// [[Rcpp::export]]
int dd_size(dd_ptr dd) {
  return dd->size();
}


// [[Rcpp::export]]
double dd_cardinality(dd_ptr dd) {
  return dd->evaluate(tdzdd::ZddCardinality<double>());
}


// [[Rcpp::export]]
std::vector<std::vector<int>> dd_solutions(dd_ptr dd, bool use_obi = true) {
  std::vector<std::vector<int>> res;  
  int n = dd->topLevel();
  
  for (auto t = dd->begin(); t != dd->end(); ++t) {
    std::vector<int> levels;
    for(int v: *t) {
      levels.push_back(n - v + (use_obi ? 1 : 0));
    }
    
    res.push_back(levels);
  }
  
  return res;
}