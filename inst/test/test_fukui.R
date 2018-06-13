library(sf)
library(dplyr)

fukui = sf::st_read("https://raw.githubusercontent.com/nyampire/jp_chome_boundary/master/TopoJSON/18-fukui-all.topojson") %>% 
  

adj = st_touches(fukui %>% group_by(CITY) %>% summarize())

Rcpp::sourceCpp("partition.cpp", embeddedR = FALSE)

res = partition_alg(adj, rep(1,17), min_pop = 1, max_pop = 16, n_part = 2, debug = FALSE)
