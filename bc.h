#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <omp.h>
#include "util.h"
#include "graph.h"

#ifndef BC_H
#define BC_H

class bc{
	public:
		path_t *local_bc;
	
	private:
		const graph *g;
		depth_t *sa;
		depth_t *tmp_sa;
		path_t *sp_count;
		path_t *tmp_bc;
		path_t *dist;
		path_t *tmp_dist;

	public:
		bc(){};
		bc(const graph *g);
		~bc(){};
	
	public:
		depth_t bfs_sssp(index_t root);
		void compute_bc(index_t root, depth_t depth_count);
		void init_traverse();
};

#endif
