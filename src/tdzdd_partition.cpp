#include <Rcpp.h>

#include "Rzdd.h"

#include "graph.hpp"
#include "frontier.hpp"
#include "util.hpp"

#include "tdzdd/DdStructure.hpp"
#include "tdzdd/DdSpec.hpp"

#include <boost/functional/hash.hpp>

typedef uint8_t int_type;

struct component {
  std::vector<int_type> current;
  weight_type weight;
  
  component(int_type v, weight_type w) 
    : current({v}),
      weight(w)
  { }
  
  void comp_union(component const& c) {
    weight += c.weight;
    
    std::vector<int_type> tmp;
    
    std::set_union(current.begin(), current.end(),
                   c.current.begin(), c.current.end(),
                   std::back_inserter(tmp));
    
    current = std::move(tmp);
  }
  
  
  bool remove(int_type v) {
    auto it = std::find(current.begin(), current.end(), v);
    
    if (it != current.end()) {
      current.erase(it);
      return true;
    }
    
    return false;
  }
  
  bool has_element(int_type v) const {
    auto const it = std::find(current.begin(), current.end(), v);
    
    return (it != current.end());
  }
  
  std::string to_string() const {
    std::string str = "{";  
    for(auto const& i : current) {
      str += std::to_string(i) + " ";
    }
    if (current.size() != 0)
      str.pop_back();
    
    str += "}(" + std::to_string(weight) + ")";
    
    
    return str;
  }
  
  friend size_t hash_value(component const& comp) {
    std::size_t seed = 0;
    boost::hash_combine(seed, boost::hash_range(comp.current.begin(), comp.current.end()));
    boost::hash_combine(seed, boost::hash_value(comp.weight));
    
    return seed;
  }
};


struct state {
  std::vector<component> comp; // Vertex components
  std::vector<std::pair<int_type,int_type>> fps;  // Forbidden pairs
  int_type cc; // Component count
  //weight_type minw;
  //weight_type maxw;
  
  state() 
    : cc(0)
  { 
    //minw = std::numeric_limits<weight_type>::infinity();
    //maxw = 0;
  }
  
  int_type find_component(int_type vertex) const {
    for(size_t i=0; i != comp.size(); ++i) {
      if (comp[i].has_element(vertex))
        return i;
    }
    
    throw std::runtime_error("Could not find vertex " + std::to_string(vertex));
    return 0;
  }
  
  std::string comp_to_string() const {
    std::string str = "[ ";
    for (auto const& s : comp) {
      str += s.to_string() + " ";
    }
    str += "]";
    
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
  
  friend size_t hash_value(state const& s) {
    std::size_t seed = 0;
    boost::hash_combine(seed, boost::hash_value(s.cc));
    boost::hash_combine(seed, boost::hash_range(s.comp.begin(), s.comp.end()));
    boost::hash_combine(seed, boost::hash_range(s.fps.begin(),  s.fps.end()));
    
    return seed;
  }
};




struct partition_zdd : public tdzdd::DdSpec<partition_zdd,state,2> {
  
public:
  graph const& g;
  frontier const& f;
  std::vector<weight_type> const& w;
  weight_type min_w;
  weight_type max_w;
  int n_part;
  int const n;
  bool debug;
  
public:
  partition_zdd(graph const& g, 
                frontier const& f,
                std::vector<weight_type> const& w,
                weight_type min_w,
                weight_type max_w,
                int n_part, 
                bool debug)
    : g(g),
      f(f),
      w(w),
      min_w(min_w),
      max_w(max_w),
      n_part(n_part),
      n(g.get_n_edges()),
      debug(debug)
  { }
  
  int getRoot(State& s) const {
    //s = state();
    
    return n;
  }
  
  int getChild(State& s, int level, int arc) const {
    
    int i = n - level;
    
    edge e = g.get_edge(i);
    
    int_type v = e.first;
    int_type w = e.second;
    
    if (arc != 0 && arc != 1)
      throw std::runtime_error("Invalid value for x.");
    
    for(int_type u : {v, w}) {
      if (!f.contains(i, u)) { // If u not in the frontier add it as a singleton component
        s.comp.push_back( component(u, this->w[u]) );
      }
    }
    
    if (debug) {
      Rcpp::Rcout << "\ni = " << i << "; arc = " << arc << "\n";
    }
    
    
    int_type Cv = s.find_component(v);
    int_type Cw = s.find_component(w);
    
    if  (Cv > Cw) { // Ensure that Cv <= Cw
      std::swap(Cv,Cw);
      std::swap(v, w);
    }
    
    auto fps_find = std::find(s.fps.begin(), s.fps.end(), std::make_pair(Cv, Cw));
    
    if (arc == 0) {
      if (Cv == Cw) {
        if (debug) Rcpp::Rcout << "Rejected: Components already connected\n";
        return 0;
      } else {
        if (fps_find == s.fps.end())
          s.fps.push_back({Cv,Cw});
      }
    } else if (arc == 1 && Cv != Cw) {
      if (Cv != Cw && fps_find != s.fps.end()) {
        if (debug) Rcpp::Rcout << "Rejected: Components in fps\n";
        return 0;
      } else {
        s.comp[Cv].comp_union(s.comp[Cw]);
        if (s.comp[Cv].weight > max_w) {
          if (debug) Rcpp::Rcout << "Rejected: Component too large\n";
          return 0;
        }
        
        s.comp.erase( s.comp.begin() + Cw );
        
        for(auto& p : s.fps) {
          if (p.first  == Cw) p.first  = Cv;
          if (p.second == Cw) p.second = Cv;
          
          if (p.first  > Cw) --p.first;
          if (p.second > Cw) --p.second;
          
          if (p.first > p.second)
            std::swap(p.first, p.second);
        }
        
        std::sort(s.fps.begin(), s.fps.end());
        s.fps.erase(
          std::unique(s.fps.begin(), s.fps.end()), 
          s.fps.end()
        );
        
        Cw = Cv;
      }
    }
    
    for(size_t j : {0, 1}) {
      int_type u  = (j == 0) ? v : w;
      int_type Cu = (j == 0) ? Cv : Cw;
      
      // Skip if u in F_{i+1} => u is not leaving frontier
      if (f.contains(i+1, u))
        continue;
      
      if (s.comp[Cu].current.size() == 1) {
        // Component is closed
        if (s.comp[Cu].weight < min_w) {
          if (debug) Rcpp::Rcout << "Rejected: Component too small\n";
          return 0;
        }
        
        s.cc++;
        if (s.cc > n_part) {
          if (debug) Rcpp::Rcout << "Rejected: Too many components\n";
          return 0;
        }
        
        std::vector<std::pair<int_type,int_type>> new_fps;
        for(auto p : s.fps) {
          if (p.first != Cu && p.second != Cu) {
            new_fps.push_back(p);
          }
        }
        s.fps = std::move(new_fps);
      }
      
      if (!s.comp[Cu].remove(u))
        throw std::runtime_error("Failed to remove u, this should never happen.");
    }
    
    if (debug) {
      Rcpp::Rcout << "cc  : " << s.cc << " " << n_part <<  "\n";
      Rcpp::Rcout << "comp: " << s.comp_to_string() << "\n";
      Rcpp::Rcout << "fps : " << s.fps_to_string() << "\n";
    }
    
    if (i == (int) g.get_n_edges()-1) {
      if (s.cc == n_part) {
        weight_type min_w = this->min_w;
        auto find_small = std::find_if(
          s.comp.begin(),  
          s.comp.end(),
          [min_w](component const& c) -> bool {return c.weight < min_w;}
        );
        
        if (find_small == s.comp.end()) {
          if (debug) Rcpp::Rcout << "Accepted\n";
          return -1;
        }
        
        throw std::runtime_error("This should never happen - component too small?");
      }
      
      if (debug) Rcpp::Rcout << "Rejected: Not enough components\n";
      return 0;
    }
    
    if (debug) Rcpp::Rcout << "Moving to next level\n";
    return level-1;
  }
  
  size_t hashCode(State const& s) const {
    return hash_value(s);
  }
};

//' partition_alg
//' 
//' Blah blah blah
//'
//' @param adj_list Adjacency list
//' @export
// [[Rcpp::export]]
Rcpp::List partition_alg(std::vector<std::vector<unsigned int>> adj_list,  
                         std::vector<weight_type> weights, 
                         weight_type min_w, weight_type max_w, int n_part,
                         bool reduce = true)
{
  
  graph g(adj_list, true); // Adjust to 0-based indexing
  frontier f(g);
  
  
  
  partition_zdd part(g, f, weights, min_w, max_w, n_part, false);
  
  tdzdd::DdStructure<2>* dd = new tdzdd::DdStructure<2>(part, true);
  if (reduce)
    dd->zddReduce();
  
  dd_ptr p(dd);
  
  Rcpp::List res = Rcpp::List::create(
    Rcpp::Named("type")  = "partition",
    Rcpp::Named("adj")   = adj_list,
    Rcpp::Named("edges") = g.to_dataframe(),
    Rcpp::Named("dd")    = p
  );
  
  res.attr("class") = "zdd";
  
  return res;
}


