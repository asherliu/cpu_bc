#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

using namespace std;

typedef struct packed_edge {
	  long int v0;
	  long int v1;
} packed_edge;

inline off_t fsize(const char *filename) {
    struct stat st; 
    if (stat(filename, &st) == 0)
        return st.st_size;
    return -1; 
}

		
int main(int argc, char** argv){
	int fd,i;
	char* ss;

	size_t file_size = fsize(argv[1]);


	fd=open( argv[1],O_CREAT|O_RDWR,00777 );



	ss = (char*)mmap(NULL,file_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

	size_t curr=0;
	size_t next=0;

	//step 1. vert_count,edge_count,
	size_t edge_count=0;
	size_t vert_count=0;
	while(next<file_size){
		if(ss[next]=='\n'){
			vert_count++;
		}
		next++;
	}
	next =0;
	while(next<file_size){
		while((ss[next]!=' ')&&(ss[next]!='\n')&&(ss[next]!='\t')){
			next++;
		}
		while((ss[next]==' ')||(ss[next]=='\n')||(ss[next]=='\t')){
			next++;
		}
		double num=atof(&ss[curr]);	
		if(num>0.0000001){
			edge_count++;
		}
		curr = next;
	}
	cout<<"edge count: "<<edge_count<<endl;
	cout<<"vert count: "<<vert_count<<endl;



	//step 2. each file size
	
	int fd1 = open( "weight.bin",O_CREAT|O_RDWR,00777 );
	ftruncate(fd1, edge_count*sizeof(double));
	double* weight = (double*)mmap(NULL,edge_count*sizeof(double),PROT_READ|PROT_WRITE,MAP_SHARED,fd1,0);

	int fd2 = open( "degree.bin",O_CREAT|O_RDWR,00777 );
	ftruncate(fd2, vert_count*sizeof(long int));
	long int* degree = (long int*)mmap(NULL,vert_count*sizeof(long int),PROT_READ|PROT_WRITE,MAP_SHARED,fd2,0);
	
	int fd3 = open( "beg.bin",O_CREAT|O_RDWR,00777 );
	ftruncate(fd3, (vert_count+1)*sizeof(long int));
	long int* begin  = (long int*)mmap(NULL,(vert_count+1)*sizeof(long int),PROT_READ|PROT_WRITE,MAP_SHARED,fd3,0);
	
	int fd4 = open( "csr.bin",O_CREAT|O_RDWR,00777 );
	ftruncate(fd4, edge_count*sizeof(long int));
	long int* adj = (long int*)mmap(NULL,edge_count*sizeof(long int),PROT_READ|PROT_WRITE,MAP_SHARED,fd4,0);
	//step 3. write to degree
	//step 4: write adjacent list 
	long int offset =0;
	long int line_id =0;
	long int col_id =0;
	curr=0;
	next=0;
	for(int i=0; i<vert_count; i++){
		degree[i]=0;
	}
	while(line_id<vert_count){
		double num=atof(&ss[curr]);	
		if(num>0.0000001){
			degree[line_id]++;
			weight[offset] = num;
			adj[offset] = col_id;
			offset++;
			
//			cout<<line_id<<"-"<<col_id<<" weight: "<<num<<endl;
		}
		col_id++;
		while((ss[next]!=' ')&&(ss[next]!='\n')&&(ss[next]!='\t')){
			next++;
			if(ss[next]=='\n'){
				line_id++;
				col_id=0;
			}
		}
		while((ss[next]==' ')||(ss[next]=='\n')||(ss[next]=='\t')){
			next++;
		}
		curr = next;
	}
//prefix sum
	begin[0]=0;
	begin[vert_count]=edge_count;
	for(size_t i=1; i<vert_count; i++){
		begin[i] = begin[i-1] + degree[i-1];
//		degree [i-1] = 0;
	}

	//step 5
	//print output as a test
	for(size_t i=0; i<vert_count; i++){
		cout<<begin[i]<<" "<<degree[i]<<" ";
		for(long int j=0; j<degree[i]; j++){
			cout<<i<<"-"<<adj[begin[i]+j]<<"-"<<weight[begin[i]+j]<<" ";
		}
		if(degree[i]>0){
			cout<<endl;
		}
	}
	cout<<begin[vert_count]<<endl;


	munmap( ss,sizeof(char)*file_size );
	munmap( adj,sizeof(long int)*edge_count );
	munmap( begin,sizeof(long int)*(vert_count+1) );
	munmap( degree,sizeof(long int)*vert_count );
	munmap( weight,sizeof(long int)*edge_count );

	return 0;
}
