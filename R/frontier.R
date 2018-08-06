
#' @export
frontier = function(x, ...) {
  UseMethod("frontier")
}

#' @export
frontier.sfc = function(sfc, ...) {
  require_sf()
  frontier(sf::st_sf(sfc), ...)
}

#' @export
frontier.sf = function(sf, ...) {
  require_sf()
  frontier(sf_to_adj(sf), ...)
}

#' @export
frontier.list = function(adj) {
  compute_frontier(adj)
}
