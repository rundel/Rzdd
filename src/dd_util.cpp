#include <Rcpp.h>

#include "tdzdd/DdStructure.hpp"

using namespace tdzdd;

//' @export
// [[Rcpp::export]]
std::string dd_to_dot(Rcpp::XPtr<tdzdd::DdStructure<2>> dd) {
  std::stringstream ss;
  dd->dumpDot(ss, "dd"); 
  return ss.str();
}

//' @export
// [[Rcpp::export]]
std::string dd_to_sapporo(Rcpp::XPtr<tdzdd::DdStructure<2>> dd) {
  std::stringstream ss;
  dd->dumpSapporo(ss); 
  return ss.str();
}

//' @export
// [[Rcpp::export]]
int dd_size(Rcpp::XPtr<tdzdd::DdStructure<2>> dd) {
  return dd->size();
}

//' @export
// [[Rcpp::export]]
double dd_cardinality(Rcpp::XPtr<tdzdd::DdStructure<2>> dd) {
  return dd->evaluate(tdzdd::ZddCardinality<double>());
}
