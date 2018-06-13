#ifndef COMB_HPP
#define COMB_HPP

// [[Rcpp::plugins(cpp11)]]

template<typename T>
std::vector<std::vector<T>> comb(std::vector<T> const& v, int k) {
  
  if (k < 0 || k > (int) v.size())
    throw std::runtime_error("Invalid value for k");
  
  std::string bitmask(k, 1);
  bitmask.resize(v.size(), 0);
  
  std::vector<std::vector<T>> res;
  do {
    std::vector<T> c;
    for (size_t i = 0; i < v.size(); ++i) {
      if (bitmask[i]) 
        c.push_back(v[i]);
    }
    
    res.push_back(c);
    
  } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
  
  return res;
}


bool contains_k_complete(std::vector<std::pair<int,int>> edges, int k) {
  int n_edges_req = (k-1)*k/2;
  
  if ( (int) edges.size() < n_edges_req )
    return false;
  
  for (auto e_set : comb(edges, n_edges_req)) {
    std::vector<int> v;
    for (auto e : e_set) {
      v.push_back(e.first);
      v.push_back(e.second);
    }
    
    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());
    
    if ((int) v.size() == k) 
      return true;
  }
  
  return false;
}

bool contains_k_complete2(std::vector<std::pair<int,int>> edges, int k) {
  int n_edges_req = (k-1)*k/2;
  
  if ( (int) edges.size() < n_edges_req )
    return false;
  
  for (auto e_set : comb(edges, n_edges_req)) {
    std::unordered_set<int> v;
    for (auto e : e_set) {
      v.insert(e.first);
      v.insert(e.second);
      
      if (v.size() > k)
        break;
    }
    
    if ((int) v.size() == k) 
      return true;
  }
  
  return false;
}

#endif