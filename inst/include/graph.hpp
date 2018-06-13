#ifndef GRAPH_HPP
#define GRAPH_HPP

// [[Rcpp::plugins(cpp11)]]

typedef std::pair<int,int> edge;

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
    std::vector<int> v;
    
    for(auto const& e : edges) {
      v.push_back(e.first);
      v.push_back(e.second);
    }
    
    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());
    
    n_vertices = v.size();
    check_edge_list();
  }
  
  graph(std::vector<std::vector<int> > const& adj_list) {
    n_vertices = 0;
    int max_vertex = 0;
    
    
    for(size_t i=0; i < adj_list.size(); i++) {
      for(size_t j=0; j < adj_list[i].size(); j++) {
        edge_list.push_back({i, adj_list[i][j]});  
        
        max_vertex = std::max(max_vertex, adj_list[i][j]);
      }
    }
    
    n_vertices = std::max(max_vertex, (int)adj_list.size());  
    check_edge_list();
  }
  
  edge make_edge(int a, int b) {
    // Ensure that first < second
    return edge(std::min(a,b), std::max(a,b));
  } 
  
  size_t get_n_vertices() const {
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
  
  bool find_in_edge_list(int val, int start = 0) const {
    for(size_t i=start; i < edge_list.size(); ++i) {
      if (edge_list[i].first == val || edge_list[i].second == val)
        return true;
    }

    return false;
  }
  
  std::vector<std::vector<int> > to_adj_list() const {
    
    std::vector<std::vector<int> > adj_list(n_vertices);
    
    for(auto e : edge_list) {
      adj_list[e.first].push_back(e.second);
      adj_list[e.second].push_back(e.first);
    }
    
    return adj_list;
  }
  
  void print_edge_list() {
    int i = 0;
    for(auto e : edge_list)
      Rcpp::Rcout << "E[" << i++ << "]: " << e.first <<  " " << e.second << "\n";
    
    Rcpp::Rcout << "\n";
  }
  
  void print_adj_list() {
    std::vector<std::vector<int> > adj = to_adj_list();
    
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