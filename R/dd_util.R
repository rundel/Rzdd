#' @export
plot_dd = function(dd) {
  if (!requireNamespace("DiagrammeR"))
    stop("Requires the DiagrammeR package for functionality")
  
  DiagrammeR::grViz(dd_to_dot(dd))
}