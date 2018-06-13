n = 4
n_part = 2
r = raster::raster(nrows=n, ncols=n)
p = sf::st_as_sf(raster::rasterToPolygons(r))

adj = sf::st_relate(p, pattern = "F***1****")



system.time({
  res = Rzdd::partition_alg(
    adj, rep(1L, length(adj)), 
    min_w = floor( length(adj)/n_part ), 
    max_w = ceiling( length(adj)/n_part ), 
    n_part = n_part,
    debug = FALSE
  )
}) 

#DiagrammeR::grViz(res)
