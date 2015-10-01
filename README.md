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
- ./cpu_bc /path/to/beg.bin /path/to/csr.bin /path/to/weight.bin num_threads


----
Graph Converter
-----------------------
Currently, we support converting a graph from matrix based text format. 
- converter in ./converter
- compile: g++ converter.cpp -o converter
- execution: ./converter /path/to/matrix_graph.txt
- You will get: *beg.bin, csr.bin, weight.bin* and degree.bin. 
- The former three files are exactly the files required by CPU BC execution

