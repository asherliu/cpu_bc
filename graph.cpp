#include "graph.h"
graph::graph(
		const char *beg_file,
		const char *csr_file,
		const char *weight_file)
{
	double tm=wtime();
	
	typedef uint64_t index_tt;
	typedef uint64_t vertex_tt;

	vert_count=fsize(beg_file)/sizeof(index_tt) - 1;
	edge_count=fsize(csr_file)/sizeof(vertex_tt);
	FILE *file=fopen(beg_file, "rb");
	if(file==NULL)
	{
		std::cout<<beg_file<<" cannot open\n";
		exit(-1);
	}

	index_tt *tmp_beg_pos = new index_tt[vert_count+1];
	index_tt ret=fread(tmp_beg_pos, sizeof(index_tt), vert_count+1, file);
	assert(ret==vert_count+1);
	fclose(file);

	file=fopen(csr_file, "rb");
	if(file==NULL)
	{
		std::cout<<csr_file<<" cannot open\n";
		exit(-1);
	}

	vertex_tt *tmp_csr = new vertex_tt[edge_count];
	ret=fread(tmp_csr, sizeof(vertex_tt), edge_count, file);
	assert(ret==edge_count);
	fclose(file);
	
	file=fopen(weight_file, "rb");
	if(file==NULL)
	{
		std::cout<<csr_file<<" cannot open\n";
		exit(-1);
	}

	weight = new path_t[edge_count];
	ret=fread(weight, sizeof(path_t), edge_count, file);
	assert(ret==edge_count);
	fclose(file);
	
	//converting to uint32_t
	beg_pos = new index_t[vert_count+1];
	csr	= new vertex_t[edge_count];
	
	for(index_t i=0;i<vert_count+1;++i)
		beg_pos[i]=(index_t)tmp_beg_pos[i];

	for(index_t i=0;i<edge_count;++i)
		csr[i]=(vertex_t)tmp_csr[i];
	
	delete[] tmp_beg_pos;
	delete[] tmp_csr;

	std::cout<<"Graph load (success): "<<vert_count<<" verts, "
		<<edge_count<<" edges "<<wtime()-tm<<" second(s)\n";
}

