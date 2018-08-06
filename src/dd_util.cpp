#include <Rcpp.h>
#include "Rzdd.h"
#include "tdzdd/DdStructure.hpp"

#include <boost/range/adaptor/reversed.hpp>

std::vector<std::string> get_lines(std::stringstream& ss, char delim = '\n') {
  std::vector<std::string> lines;
  
  std::string token;
  while (std::getline(ss, token, delim)) {
    lines.push_back(token);
  }
  
  return lines;
}


// [[Rcpp::export]]
std::vector<std::string> dd_to_dot(dd_ptr dd) {
  std::stringstream ss;
  dd->dumpDot(ss, "dd");
  
  return get_lines(ss);
}


// [[Rcpp::export]]
std::vector<std::string> dd_to_sapporo(dd_ptr dd) {
  std::stringstream ss;
  dd->dumpSapporo(ss); 
  
  return get_lines(ss);
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
    for(int v: boost::adaptors::reverse(*t)) {
      levels.push_back(n - v + (use_obi ? 1 : 0));
    }
    
    res.push_back(levels);
  }
  
  return res;
}