require_sf = function() {
  if (!requireNamespace("sf", quietly = TRUE))
    stop("sf package is required for this functionality")
}

#' @export
sf_to_adj = function(sf, pattern = "F***1****") {
  require_sf()
  r = sf::st_relate(sf, pattern = pattern)
  class(r) = NULL
  
  r
}