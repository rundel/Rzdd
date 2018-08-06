/*
 * enumpart
 * by Jun Kawahara
 * Copyright (c) 2018 Jun Kawahara
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <climits>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>

#include <tdzdd/DdSpecOp.hpp>
#include <tdzdd/DdStructure.hpp>
#include <tdzdd/util/Graph.hpp>

#include <enumpart/GraphPartitionSpec.hpp>
#include <enumpart/ComponentWeightSpec.hpp>
#include <enumpart/ComponentRatioSpec.hpp>

#include "Rzdd.h"

tdzdd::Graph tzdd_graph_from_edgelist(Rcpp::DataFrame const& edges) {
    Graph g;
    
    if (edges.size() != 2)
        throw std::runtime_error("ERROR: expected two columns.");

    Rcpp::IntegerVector from = edges[0];
    Rcpp::IntegerVector to   = edges[1];
    
    for(size_t i=0; i != from.size(); ++i) {
      g.addEdge(std::to_string(from[i]), std::to_string(to[i]));   
    }
    g.update();
    
    return g;
}

Rcpp::DataFrame tzdd_graph_to_edgelist(Graph const& g) {
    std::vector<unsigned int> from, to;
    
    
    for (int a = 0; a < g.edgeSize(); ++a) {
        tdzdd::Graph::EdgeInfo const& e = g.edgeInfo(a);
        
        from.push_back(e.v1);
        to.push_back(e.v2);
    }
    
    return Rcpp::DataFrame::create(
        Rcpp::Named("from") = from,
        Rcpp::Named("to") = to
    );
}


tdzdd::Graph tdzdd_graph_from_adj(std::vector<std::vector<unsigned int>> const& adj_list) {
    Graph g;
    for(size_t i=0; i != adj_list.size(); ++i) {
        for(size_t j=0; j != adj_list[i].size(); ++j) {
            g.addEdge(std::to_string(i+1), std::to_string(adj_list[i][j]));    
        }
    }
    g.update();
    
    return g;
}


// [[Rcpp::export]]
Rcpp::List enum_part_alg(std::vector<std::vector<unsigned int>> adj, int n_part = 2,
                         bool lookahead = true, bool noloop = false,
                         bool verbose = true, bool use_openmp = true)
{
    if (verbose) {
        MessageHandler::showMessages();
    }
    
    tdzdd::MessageHandler mh;
    mh.begin("started\n");

    tdzdd::Graph g = tdzdd_graph_from_adj(adj);
    
    int const m = g.vertexSize();
    int const n = g.edgeSize();

    mh << "#vertex = " << m << ", #edge = " << n << "\n";

    if (g.edgeSize() == 0)
        throw std::runtime_error("ERROR: The graph is empty.");

    GraphPartitionSpec gpspec(g, n_part, noloop, lookahead);
    
    tdzdd::DdStructure<2>* dd = new tdzdd::DdStructure<2>(gpspec, use_openmp);
    dd->zddReduce();
    
    dd_ptr p(dd);
    
    Rcpp::List res = Rcpp::List::create(
        Rcpp::Named("type")   = "enumpart",
        Rcpp::Named("adj")    = adj,
        Rcpp::Named("edges")  = tzdd_graph_to_edgelist(g),
        Rcpp::Named("n_part") = n_part,
        Rcpp::Named("dd")     = p,
        Rcpp::Named("lookahead") = lookahead,
        Rcpp::Named("noloop") = noloop
    );
    
    res.attr("class") = "zdd";
    
    mh.end("finished\n");
    
    return res;   
}    


// [[Rcpp::export]]
Rcpp::List ratio_constrain(Rcpp::List zdd, std::vector<unsigned int> weights, double ratio) {
    dd_ptr dd = zdd["dd"];
    tdzdd::Graph g = tdzdd_graph_from_adj(zdd["adj"]);
    
    if (weights.size() != g.vertexSize())
        throw std::runtime_error("ERROR: Incorrect number of vertex weights provided.");
    
    
    unsigned int sum = std::accumulate(weights.begin(), weights.end(), 0);
    int k = zdd["n_part"];
    
    int lower = static_cast<int>(floor(static_cast<double>(sum) / (ratio * (k - 1) + 1)));
    int upper = static_cast<int>(ceil(ratio * static_cast<double>(sum) / (ratio + (k - 1))));
    
    ComponentRatioSpec crspec(g, weights, lower, upper,
                              ratio, zdd["noloop"], zdd["lookahead"]);
    
    dd->zddSubset(crspec);
    dd->zddReduce();
    
    zdd["dd"] = dd;
    zdd["constrained"] = "ratio";
    zdd["ratio"] = ratio;
    zdd["lower"] = lower;   
    zdd["upper"] = upper;
    
    zdd.attr("class") = "zdd";
    
    return zdd; 
}


// [[Rcpp::export]]
Rcpp::List min_max_constrain(Rcpp::List zdd, std::vector<unsigned int> weights, 
                             unsigned int lower, unsigned int upper) {
    dd_ptr dd = zdd["dd"];
    tdzdd::Graph g = tdzdd_graph_from_adj(zdd["adj"]);
    
    if (weights.size() != g.vertexSize())
        throw std::runtime_error("ERROR: Incorrect number of vertex weights provided.");
    
    unsigned int sum = std::accumulate(weights.begin(), weights.end(), 0);
    int k = zdd["n_part"];
    
    ComponentWeightSpec cwspec(g, weights, lower, upper, zdd["noloop"], zdd["lookahead"]);

    dd->zddSubset(cwspec);
    dd->zddReduce();
    
    zdd["dd"] = dd;
    zdd["constrained"] = "min/max";
    zdd["lower"] = lower;   
    zdd["upper"] = upper;
    
    zdd.attr("class") = "zdd";
    
    return zdd; 
}
