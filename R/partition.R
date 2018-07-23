require_sf = function() {
  if (!requireNamespace("sf", quietly = TRUE))
    stop("sf package is required for this functionality")
}

#' @export
partition = function(x, ...) {
  UseMethod("partition")
}

#' @export
partition.sfc = function(sfc_obj, ...) {
  require_sf()
  partition(sf::st_sf(sfc_obj), ...)
}

#' @export
partition.sf = function(sf_obj, n=2,
                        pattern = "F***1****",
                        weights=rep(1,nrow(sf_obj)),
                        min_w = floor(nrow(sf_obj)/n),
                        max_w = ceiling(nrow(sf_obj)/n))
{
  require_sf()
  stopifnot(min_w <= max_w)
  
  adj = sf::st_relate(sf_obj, pattern = pattern)
  
  zdd = partition_alg(adj, weights, min_w, max_w, n_part, reduce = TRUE)
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
      
      igraph::components(g)[["membership"]]
    }
  )
}