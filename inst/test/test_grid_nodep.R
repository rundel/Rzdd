n=3
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

res = Rzdd::partition(adj, 2)

res

Rzdd::partition_labels(res)
