#include "wtime.h"
#include "graph.h"
#include "bc.h"


template<typename T, typename IT>
void print(T *data, IT count)
{
	IT i=0;
	for(i=0;i<count;++i)
		std::cout<<i<<" "<<data[i]<<"\n";
	std::cout<<"\n";
}

int main( int args, char **argv)
{
	std::cout<<"Input: ./exe /path/to/beg /path/to/csr"
			<<"/path/to/weight thread-count\n";
	if(args != 5) exit(-1);
	const char *beg_filename = argv[1];
	const char *csr_filename = argv[2];
	const char *weight_filename = argv[3];
	const int thd_count = atoi(argv[4]);

	graph *g 				= new graph(beg_filename, csr_filename, weight_filename);
	path_t *glb_bc	= new path_t[g->vert_count];
	char *lock			= new char[g->vert_count];
	memset(glb_bc, 0, sizeof(path_t)*g->vert_count);
	memset(lock, 0, sizeof(char)*g->vert_count);
		
	double btm= wtime();
	#pragma omp parallel \
	num_threads(thd_count)
	{
		index_t report=1;

		index_t tid=omp_get_thread_num();
		index_t step=g->vert_count/thd_count;
		const index_t beg=tid*step;
		const index_t end=(tid==thd_count-1 ? g->vert_count:beg+step);
		bc *bc_inst = new bc((const graph *)g);

		for(index_t i=beg;i<end;i++)
		{
//			if(tid==0 && report<=i)
//			{
//				std::cout<<i<<" finished with "<<wtime()-btm<<" seconds\n";
//				report <<=1;
//			}
			if(g->beg_pos[i]==g->beg_pos[i+1])continue;
			bc_inst->init_traverse();	
			depth_t tdepth=bc_inst->bfs_sssp(i);
			bc_inst->compute_bc(i,tdepth);
		}
		
		for(index_t i=0;i<g->vert_count;++i)
		{
			LOCK(i, lock);
			glb_bc[i]+=bc_inst->local_bc[i];
			UNLOCK(i, lock);
		}
	}

	std::cout<<"Total time: "<<wtime()-btm<<" seconds\n";
	std::cout<<"BC value: \n";
	print<path_t, index_t>(glb_bc, g->vert_count);
	return 0;
}
