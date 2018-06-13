#ifndef ZDD_NODE_HPP
#define ZDD_NODE_HPP

// [[Rcpp::plugins(cpp11)]]

#include "component.hpp"

struct zdd_node {
  int label;
  std::string why;
  std::vector<component> comp; // Vertex components
  std::vector<std::pair<int,int>> fps;  // Forbidden pairs
  int cc; // Component count
  int arc[2];
  double minw;
  double maxw;
  bool track;
  
  zdd_node () 
    : label(-1),
      cc(0),
      track(false)
  { 
    minw = std::numeric_limits<double>::infinity();
    maxw = 0;
    
    arc[0] = -3;
    arc[1] = -3;
  }
  
  bool operator ==(zdd_node const& n) const {
    return    cc == n.cc
           && fps.size()  == n.fps.size()
           && comp.size() == n.comp.size()
           && fps  == n.fps
           && comp == n.comp;
  }
  
  int find_component(int vertex, bool strict = true) const {
    for(size_t i=0; i != comp.size(); ++i) {
      if (comp[i].has_element(vertex))
        return i;
    }
    
    if (strict)
      throw std::runtime_error(
          "Could not find vertex " +
            std::to_string(vertex) +
            " (this shouldn't happen).\n" +
            "Comp1: " + comp_to_string() + "\n" +
            "Comp2: " + comp_to_string(false) + "\n"
      );
    
    return -1;
  }
  
  std::string weight_to_string() const {
    std::string str = "{ ";
    for (auto const& c : comp) {
      str += std::to_string(c.weight) + " ";
    }
    str += "}";
    
    return str;
  }
  
  std::string fps_to_string() const {
    std::string str = "[ ";
    for (auto const& p : fps) {
      str += "{" + std::to_string(p.first) + " " + std::to_string(p.second) + "} ";
    }
    str += "]";
    
    return str;
  }
  
  std::string comp_to_string(bool use_current = true) const {
    std::string str = "[ ";
    for (auto const& s : comp) {
      str += s.to_string(use_current) + " ";
    }
    str += "]";
    
    return str;
  }
  
  void print() const {
    Rcpp::Rcout << "Label: " << label << "\n";
    Rcpp::Rcout << "cc   : " << cc << "\n";
    Rcpp::Rcout << "Comp1: " << comp_to_string(true) << "\n";
    Rcpp::Rcout << "Comp2: " << comp_to_string(false) << "\n";
    Rcpp::Rcout << "fps  : " << fps_to_string()  << "\n";
    Rcpp::Rcout << "arc 0: " << arc[0] << "\n";
    Rcpp::Rcout << "arc 1: " << arc[1] << "\n";
  }
};

// Based on https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
namespace std {
  template <>
  struct hash<zdd_node> {
    size_t operator()(zdd_node const& n) const {
      std::size_t seed = 0;
      boost::hash_combine(seed, boost::hash_value(n.cc));
      boost::hash_combine(seed, boost::hash_range(n.comp.begin(), n.comp.end()));
      boost::hash_combine(seed, boost::hash_range(n.fps.begin(), n.fps.end()));
      
      return seed;
    }
  };
}

#endif