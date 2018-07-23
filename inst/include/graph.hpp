#ifndef GRAPH_HPP
#define GRAPH_HPP

// [[Rcpp::plugins(cpp11)]]

typedef std::pair<unsigned int,unsigned int> edge;

// Assumes the graph is undirected
class graph {
private:
  int n_vertices;
  std::vector<edge> edge_list;

public:

  void check_edge_list() {
    for(auto& e : edge_list) {
      if (e.first > e.second)
        std::swap(e.first, e.second);
    }
    
    std::sort(edge_list.begin(), edge_list.end());
    edge_list.erase(
      std::unique(edge_list.begin(), edge_list.end()), 
      edge_list.end()
    );
  }
  
  graph(std::vector<edge> const& edges) 
    : edge_list(edges) 
  { 
    std::vector<unsigned int> v;
    
    for(auto const& e : edges) {
      v.push_back(e.first);
      v.push_back(e.second);
    }
    
    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());
    
    n_vertices = v.size();
    check_edge_list();
  }
  
  graph(std::vector<std::vector<unsigned int> > const& adj_list, bool from_obi = true) {
    unsigned int v_max = 0;
    
    
    for(size_t i=0; i < adj_list.size(); i++) {
      for(size_t j=0; j < adj_list[i].size(); j++) {
        unsigned int v_from = i;
        unsigned int v_to = adj_list[i][j] - (from_obi ? 1 : 0);
        
        edge_list.push_back({v_from, v_to});  
        
        v_max = std::max(v_max, v_to);
      }
    }
    
    n_vertices = std::max(v_max, (unsigned int) adj_list.size());  
    check_edge_list();
  }
  
  edge make_edge(unsigned int a, unsigned int b) {
    // Ensure that first < second
    return edge(std::min(a,b), std::max(a,b));
  } 
  
  unsigned int get_n_vertices() const {
    return n_vertices;
  }
  
  size_t get_n_edges() const {
    return edge_list.size();
  }
  
  edge get_edge(int i) const {
    return edge_list[i];
  }

  std::vector<edge> const& get_edge_list() {
    return edge_list;
  }
  
  bool find_in_edge_list(unsigned int val, unsigned int start = 0) const {
    for(size_t i=start; i < edge_list.size(); ++i) {
      if (edge_list[i].first == val || edge_list[i].second == val)
        return true;
    }

    return false;
  }
  
  std::vector<std::vector<unsigned int> > to_adj_list() const {
    
    std::vector<std::vector<unsigned int> > adj_list(n_vertices);
    
    for(auto e : edge_list) {
      adj_list[e.first].push_back(e.second);
      adj_list[e.second].push_back(e.first);
    }
    
    return adj_list;
  }
  
  Rcpp::DataFrame to_dataframe(bool return_obi = true) {
    std::vector<unsigned int> edge, v_from, v_to;
    
    for(size_t i=0; i!=edge_list.size(); ++i) {
      edge.push_back(   i                   + (return_obi ? 1 : 0) );
      v_from.push_back( edge_list[i].first  + (return_obi ? 1 : 0) );
      v_to.push_back(   edge_list[i].second + (return_obi ? 1 : 0) );
    }
    
    return Rcpp::DataFrame::create(
      Rcpp::Named("edge") = edge,
      Rcpp::Named("from") = v_from,
      Rcpp::Named("to") = v_to
    );
  }
  
  void print_edge_list() {
    unsigned int i = 0;
    for(auto e : edge_list)
      Rcpp::Rcout << "E[" << i++ << "]: " << e.first <<  " " << e.second << "\n";
    
    Rcpp::Rcout << "\n";
  }
  
  void print_adj_list() {
    std::vector<std::vector<unsigned int> > adj = to_adj_list();
    
    for(size_t i=0; i!=adj.size(); ++i) {
      Rcpp::Rcout << "V[" << i << "]:";
      for(auto e : adj[i])
        Rcpp::Rcout << " " << e;
      
      Rcpp::Rcout << "\n";
    }
    
    Rcpp::Rcout << "\n";
  }
};

#endif