library(magrittr)
library(ggplot2)

n=5
r = raster::raster(nrows=n, ncols=n, xmn=0, xmx=n, ymn=0, ymx=n)
p = sf::st_as_sf(raster::rasterToPolygons(r))
p$layer = NULL

#res = Rzdd::partition(p, 2)
#res = Rzdd::enum_part(p, lower=floor(n^2/2), upper=ceiling(n^2/2))
res = Rzdd::enum_part(p, ratio=1)

res

#plot(res)

solutions = partition_labels(res, sample_n = 100)



plots = purrr::map(
  solutions,
  function(solution) {
    ggplot(p, aes(fill=as.factor(solution))) +
      geom_sf() +
      theme_void() +
      guides(fill=FALSE)
  }
)

pdf("~/Desktop/test.pdf")
cowplot::plot_grid(plotlist = plots, scale = 0.95)
dev.off()
