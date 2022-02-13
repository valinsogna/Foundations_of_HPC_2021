#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include <time.h>

#define DEFAULT 1000000
#define SEED    918273

int main ( int argc, char **argv)
{
    
    long long int M=0;
    int           nthreads;
    double        pi;
    
    
    
    #pragma omp parallel  
    #pragma omp master
    nthreads = omp_get_num_threads();

    long long int N = (argc > 1 ? atoll(argv[1]) : DEFAULT ) ;
    printf("omp calculation with %d threads\nN=%Ld\n",
	   nthreads ,N);

    double timing = omp_get_wtime(); // outside parallel region
    #pragma omp parallel
    {
      int myid = omp_get_thread_num();
      double x, y ;
      srand48(SEED*(myid+1)); //it's thread safe

     #pragma omp for reduction(+:M)
      for( long long unsigned i = 0; i < N; i++)
	{
    //man srand48
    //drand48 it's thread safe, but it's multi-thread unsafe  in sense that it's race!!!!!!NOT a parallel random generator
	  //These functions have an internal status (a value of the entropy) and in parallel each thread should have a unique status specifically!
    x = drand48(); //Internal status of entropy must differ among threads: use erand48, not drand48!
	  y = drand48();
	  M += ((x*x + y*y) < 1.0);
	}
    }
    
    timing = omp_get_wtime() - timing; // outside parallel region: walltime
    printf("Estimation of pi: %1.9f\n Walltime:%g\n",
	   (4.0*(double)M)/N, timing );
      
    return 0;
}
