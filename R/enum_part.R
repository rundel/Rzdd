#' @export
enum_part = function(x, ...) {
  UseMethod("enum_part")
}

#' @export
enum_part.sfc = function(sfc, ...) {
  require_sf()
  enum_part(sf::st_sf(sfc), ...)
}

#' @export
enum_part.sf = function(sf, labels = NULL, ...) {
  require_sf()
  if (is.null(labels))
    labels = as.character(seq(1,nrow(sf)))
  else
    labels = sf[[labels]]
  
  enum_part(sf_to_adj(sf), labels = labels, ...)
}

#' @export
enum_part.list = function(adj, labels = as.character(seq(1,length(adj))),
                          n_part = 2, weights = rep(1, length(adj)),
                          ratio = NULL, lower = NULL, upper = NULL,
                          use_openmp = TRUE, verbose=TRUE)
{
  zdd = enum_part_alg(adj, n_part=n_part, use_openmp=use_openmp, verbose=verbose, reduce=TRUE)
  
  if (!is.null(ratio)) {
    zdd = ratio_constrain(zdd, weights, ratio)
  } else if (!is.null(lower) & !is.null(upper)) {
    stopifnot(lower <= upper)
    zdd = min_max_constrain(zdd, weights, lower, upper)
  }
  
  zdd
}
