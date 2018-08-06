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
enum_part.sf = function(sf, ...) {
  require_sf()
  enum_part(sf_to_adj(sf), ...)
}

#' @export
enum_part.list = function(adj, n_part = 2, weights = rep(1, length(adj)),
                          ratio = NULL, lower = NULL, upper = NULL,
                          use_openmp = TRUE)
{
  zdd = enum_part_alg(adj, n_part=n_part, use_openmp=TRUE)
  
  if (!is.null(ratio)) {
    zdd = ratio_constrain(zdd, weights, ratio)
  } else if (!is.null(lower) & !is.null(upper)) {
    stopifnot(lower <= upper)
    zdd = min_max_constrain(zdd, weights, lower, upper)
  }
  
  zdd
}
