# Betweeenness centrality (BC) on CPU

CPU implementation for BC on weighted graph
**Contributor**: Hang Liu (asherliu@gwu.edu)

---
Environment requirement:
---------------------
g++, openmp

----
Compile
---------
- make

---
Execution
--------------
- ./cpu_bc /path/to/begin.bin /path/to/csr.bin /path/to/weight.bin num_threads


----
Graph Converter
-----------------------
Currently, we support from matrix based graph converter. 
- converter in ./converter
- compile: g++ 
