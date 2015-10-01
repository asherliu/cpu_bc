# Betweenness centrality (BC) on CPU

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
Currently, we support converting a graph from matrix based text format. (credit to Yang Hu)
- Source code of converter is in ./converter folder
- Compile: **g++ converter.cpp -o converter**
- Execution: **./converter /path/to/matrix_graph.txt (please make sure there is no blanket lines in this file)**
- You will get four files: **beg.bin, csr.bin, weight.bin** and degree.bin. 
- The former three files are exactly the files required by CPU BC execution

