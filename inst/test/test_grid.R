library(magrittr)
library(ggplot2)

n=2
r = raster::raster(nrows=n, ncols=n, xmn=0, xmx=n, ymn=0, ymx=n)
p = sf::st_as_sf(raster::rasterToPolygons(r))
p$layer = NULL

res = Rzdd::enum_part(p, ratio=1, verbose=FALSE)

res

#plot(res)

Rzdd:::dd_solutions(res$dd)

solutions = Rzdd:::partition_labels(res)



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
