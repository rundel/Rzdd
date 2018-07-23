library(magrittr)
library(ggplot2)

n=3
r = raster::raster(nrows=n, ncols=n, xmn=0, xmx=n, ymn=0, ymx=n)
p = sf::st_as_sf(raster::rasterToPolygons(r))
p$layer = NULL

res = Rzdd::partition(p, 2)

plot(res)

res

solutions = partition_labels(res)

stop()

plots = purrr::map(
  solutions,
  function(solution) {
    ggplot(p, aes(fill=as.factor(solution))) +
      geom_sf() +
      theme_void() +
      guides(fill=FALSE)
  }
)

#pdf("~/Desktop/test.pdf")
cowplot::plot_grid(plotlist = plots, scale = 0.95)
#dev.off()
