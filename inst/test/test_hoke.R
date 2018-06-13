library(sf)
library(dplyr)

hoke = st_read("../data/Hoke_Cumberland_shapefile.shp") %>% 
  select(vtd, pop)
 
hoke = bind_cols(hoke,  st_centroid(hoke) %>% st_coordinates() %>% as.data.frame()) %>%
  arrange(X)

pop = hoke$pop
total_pop = sum(pop)
min_pop = (total_pop / 2) * 0.95
max_pop = (total_pop / 2) * 1.05

adj = st_touches(hoke)

Rcpp::sourceCpp("partition.cpp", embeddedR = FALSE)

res = partition_alg(adj, pop, min_pop = min_pop, max_pop = max_pop, n_part = 2, debug = FALSE)
