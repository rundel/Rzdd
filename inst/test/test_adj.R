adj = list(
  c(2L, 4L), 
  c(1L, 3L, 5L), 
  c(2L, 6L), 
  c(1L, 5L, 7L), 
  c(2L, 4L, 6L, 8L), 
  c(3L, 5L, 9L), 
  c(4L, 8L), 
  c(5L, 7L, 9L), 
  c(6L, 8L)
)

part = Rzdd::enum_part(adj)
sols = Rzdd::partition_labels(part)

graph = tidygraph::as_tbl_graph(
  igraph::graph_from_adj_list(adj)
) %>%
  tidygraph::activate(nodes) %>%
  dplyr::mutate(
    name = as.character(1:dplyr::n())
  )

theme_blank = ggplot2::theme(
  axis.line=element_blank(),
  axis.text.x=element_blank(),
  axis.text.y=element_blank(),
  axis.ticks=element_blank(),
  axis.title.x=element_blank(),
  axis.title.y=element_blank(),
  legend.position="none",
  panel.background=element_blank(),
  #panel.border=element_blank(),
  panel.grid.major=element_blank(),
  panel.grid.minor=element_blank(),
  plot.background=element_blank()
)



plots = purrr::map(
  sols,
  function(s) { 
    ggraph(graph, layout = "kk") +
      geom_edge_link() +
      geom_node_point(aes(color = as.factor(s)), size=8) +
      geom_node_text(aes(label = name)) +
      theme_blank
  }
)

cowplot::plot_grid(plotlist = plots, scale = 0.8)

