#include "bc.h"

bc::bc(const graph *g):g(g)
{
	size_t align = 512;	
	size_t depth_sz=sizeof(depth_t)*g->vert_count;
	size_t path_sz=sizeof(path_t)*g->vert_count;

	if(	posix_memalign((void **)&sa, align, depth_sz) ||
			posix_memalign((void **)&local_bc, align, path_sz) ||
			posix_memalign((void **)&sp_count, align, path_sz)||
			posix_memalign((void **)&dist, align, path_sz)||
			posix_memalign((void **)&tmp_dist, align, path_sz)||
			posix_memalign((void **)&tmp_sa, align, depth_sz)||
			posix_memalign((void **)&tmp_bc, align, path_sz))
	{perror("posix_memalign"); exit(-1);}

	memset(local_bc, 0, path_sz);
	for(index_t k=0;k<g->vert_count;++k){
		tmp_dist[k]=INFTY;
		tmp_sa[k]=UNVIS;
	}
}

void bc::compute_bc(index_t root, depth_t depth_count)
{
	for(depth_t i=depth_count;i>=0;--i)
	{
		for(vertex_t vert_id = 0; vert_id<g->vert_count; vert_id++)
		{
			if(sa[vert_id] == i)
			{
				path_t my_dist=dist[vert_id];
				index_t my_beg = g->beg_pos[vert_id];
				index_t my_end = g->beg_pos[vert_id + 1];
				path_t tbc = 0;
				path_t tpath=sp_count[vert_id];
				for(; my_beg<my_end; my_beg ++)
				{
					vertex_t nebr=g->csr[my_beg];
					if(my_dist+g->weight[my_beg] == dist[nebr])
						tbc+=tpath*(1+tmp_bc[nebr])/sp_count[nebr];
				}
				tmp_bc[vert_id] = tbc;
			}
		}
	}
	
	tmp_bc[root]=0;
	for(vertex_t vert_id=0; vert_id<g->vert_count; vert_id++)
		local_bc[vert_id] += tmp_bc[vert_id];
}

depth_t bc::bfs_sssp(
	index_t root
)
{
	sa[root] = 0;
	sp_count[root] = 1;
	depth_t level = 0;
	dist[root]=0;
	while(true)
	{
		double ltm= wtime();
		index_t front_count = 0;
		for(vertex_t vert_id = 0; vert_id<g->vert_count; vert_id++)
		{
			if(sa[vert_id] == level)
			{
				index_t my_beg = g->beg_pos[vert_id];
				index_t my_end = g->beg_pos[vert_id + 1];

				for(; my_beg<my_end; my_beg++)
				{
					vertex_t nebr=g->csr[my_beg];
					path_t weit=g->weight[my_beg];

					if(dist[nebr]>dist[vert_id]+weit)
					{
						dist[nebr]=dist[vert_id]+weit;
						sp_count[nebr]=0; //prior parent is wrong
						sa[nebr]=level+1;
						front_count++;
					}

					if(dist[nebr]==dist[vert_id]+weit)
						sp_count[nebr]+=sp_count[vert_id];
				}
			}
		}
//		std::cout<<"Level "<<(int) level<<": "<<front_count<<" "
//							<<wtime() - ltm<<"\n";
		if(front_count == 0) break;
		level ++;
	}
	
	return level+1;

}

void bc::init_traverse()
{
	size_t depth_sz=sizeof(depth_t)*g->vert_count;
	size_t path_sz=sizeof(path_t)*g->vert_count;
	
	memset(sp_count, 0, path_sz);
	memset(tmp_bc, 0, path_sz);
	memcpy(sa,tmp_sa, depth_sz);
	memcpy(dist,tmp_dist,path_sz);
}
