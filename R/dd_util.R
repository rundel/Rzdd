save_zdd = function(zdd, filename, method = "sapporo") {
  lines = dd_to_sapporo(zdd$dd)
  writeLines(lines, filename)
}

load_zdd = function(filename) {
  stop()
}


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
  
  if (!is.null(zdd$constrained)) {
    cat("  Constraint: ", zdd$constrained, "\n", sep="")
    if (!is.null(zdd$ratio))
      cat("    * Ratio: ", zdd$ratio, "\n", sep="")
    cat("    * Lower: ", zdd$lower, "\n", sep="")
    cat("    * Upper: ", zdd$upper, "\n", sep="")
  }
}