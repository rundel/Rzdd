#' @export
plot.zdd = function(zdd) {
  if (!requireNamespace("DiagrammeR"))
    stop("Requires the DiagrammeR package for functionality")
  
  DiagrammeR::grViz(dd_to_dot(zdd$dd))
}

#' @export
print.zdd = function(zdd) {
  cat("Original graph:\n")
  cat("  # of nodes: ", length(zdd$adj), "\n", sep="")
  cat("  # of edges: ", nrow(zdd$edges), "\n", sep="")
  
  cat("ZDD (", zdd$type,"):\n", sep="")
  cat("  # of nodes    : ", dd_size(zdd$dd), "\n", sep="")
  cat("  # of solutions: ", dd_cardinality(zdd$dd), "\n", sep="")
}