require_sf = function() {
  if (requireNamespace("sf", quietly = TRUE))
    stop("sf package is required for this functionality")
}

#' @export
partition = function(x, ...) {
  UseMethod("partition", x)
}

partition.sfc = function(sfc_obj, ...) {
  require_sf()
  partition(sf::st_sf(sfc_obj), ...)
}

partition.sf = function(sf_obj, n=2, 
                        weights=rep(1,nrow(sf_obj)), 
                        min_w = floor(nrow(adj)/n), 
                        max_w = ceiling(nrow(adj)/n))
{
  require_sf()
  stopifnot(min_w <= max_w)
  
  
  adj = sf::st_relate(sf_obj, pattern = "F***1****")
  
  zdd = partition_alg(adj, weights, min_w, max_w, n_part, reduce = TRUE)
}


#' @export
partition_labels = function(edges, solution) {
  el = edges[solution,c("from","to")]
  g = igraph::graph_from_edgelist(as.matrix(el), FALSE)
  
  igraph::components(g)[["membership"]]
}