#ifndef COMPONENT_HPP
#define COMPONENT_HPP

// [[Rcpp::plugins(cpp11)]]

#include <boost/functional/hash.hpp>

struct component {
  std::vector<int> current;
  //std::vector<int> removed;
  double weight;

  component(double w) 
    : weight(w)
  { }
    
  component(int v, double w) 
    : current({v}),
      weight(w)
  { }
  
  bool operator == (component const& c) const {
    return    current == c.current 
           && weight == c.weight;
  }
  
  void comp_union(component const& c) {
    weight += c.weight;
    
    std::vector<int> new_current;
    //std::vector<int> new_removed;
    
    std::set_union(current.begin(), current.end(),
                   c.current.begin(), c.current.end(),
                   std::back_inserter(new_current));
    
    std::sort(new_current.begin(), new_current.end());
    
    new_current.erase( 
      std::unique( new_current.begin(), new_current.end() ), 
      new_current.end() 
    );
    
    current = new_current;
    
    // std::set_union(removed.begin(), removed.end(),
    //                c.removed.begin(), c.removed.end(),
    //                std::back_inserter(new_removed));
    // std::sort(new_removed.begin(), new_removed.end());
    // 
    // 
    // removed = new_removed;
  }
  
  
  bool remove(int v) {
    auto it = std::find(current.begin(), current.end(), v);
    
    if (it != current.end()) {
      current.erase(it);
      //removed.push_back(v);
      //std::sort(removed.begin(), removed.end());
      
      return true;
    }
    
    return false;
  }
  
  bool has_element(int v) const {
    auto const it = std::find(current.begin(), current.end(), v);
    
    return (it != current.end());
  }
  
  std::string to_string(bool use_current = true) const {
    //auto& vec = (use_current) ? current : removed;
    auto& vec = (use_current) ? current : current;
    
    std::string str = "{";  
    for(auto const& i : vec) {
      str += std::to_string(i) + " ";
    }
    if (vec.size() != 0)
      str.pop_back();
    
    str += "}";

    return str;
  }
  
  friend size_t hash_value(component const& k) {
    std::size_t seed = 0;
    boost::hash_combine(seed, boost::hash_range(k.current.begin(), k.current.end()));
    boost::hash_combine(seed, boost::hash_value(k.weight));
    
    return seed;
  }
};

// Based on https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
namespace std {
  template <>
  struct hash<component> {
    size_t operator()(component const& k) const {
      return hash_value(k);
    }
  };
}

#endif