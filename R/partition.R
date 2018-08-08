#' @export
partition = function(x, ...) {
  UseMethod("partition")
}

#' @export
partition.sfc = function(sfc, ...) {
  require_sf()
  partition(sf::st_sf(sfc), ...)
}

#' @export
partition.sf = function(sf, ...) {
  require_sf()
  partition(sf_to_adj(sf), ...)
}

#' @export
partition.list = function(adj, n=2,
                          weights=rep(1,length(adj)),
                          min_w = floor(length(adj)/n),
                          max_w = ceiling(length(adj)/n))
{
  stopifnot(min_w <= max_w)
  stopifnot(n <= length(adj))
  
  partition_alg(adj, weights, min_w, max_w, n_part=n, reduce = TRUE)
}



#' @export
partition_labels = function(res, sample_n = NULL) {
  solutions = dd_solutions(res$dd, use_obi = TRUE)
  
  if (!is.null(sample_n)) {
    solutions = sample(solutions, size = sample_n, replace = FALSE)
  }
  
  lapply(
    solutions,
    function(solution) {
      el = res$edges[solution,c("from","to")]
      g = igraph::graph_from_edgelist(as.matrix(el), FALSE)
      comps = igraph::components(g)[["membership"]]
      
      comps[res$labels] # reorder to match original data
    }
  )
}
