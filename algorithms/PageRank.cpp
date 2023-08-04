//
// Created by farouk on 30/06/23.
//
#include <iostream>
#include "../graph-creation/EC.h"
#include "PageRank.h"
#include <chrono>
#include <algorithm>
#include <fstream>
#include <numeric>
#include "../graph-creation/getDstFile.h"
#include <immintrin.h>
#include <cassert>


#define d 0.85f
//#define cilk_for for


void parallelPageRank(const std::string& path, const int n, const int nb_iteration){


    auto x = getDstFile(path);
    int *dst = x.first ,nb_edges = x.second ;

    ExtendedPairEdgeCentric g = BranchlessCreateGraphFromFilePageRank(path,n,nb_edges);
    std::cout << "loaded the graph in memory " << '\n' ;


    // TODO : construct an float array of inverses of degrees


    auto start = std::chrono::high_resolution_clock::now();

    // allocate two arrays of size n
    // one contains PR for previous iteration
    // and one contains PR for current iteration
    //std::cout << n/8 +1 << '\n' ;
    auto* previousPR = static_cast<float *>(std::aligned_alloc(alignof(__m256),(n/8+1) * sizeof(__m256 )));
    auto* PR = static_cast<float *>(std::aligned_alloc(alignof(__m256),(n/8+1) * sizeof(__m256 )));
    float * temp ;

    auto * inverse_out_degree = static_cast<float *>(malloc(n * sizeof(float )));
    int i = 0;
    for (; i < n-8; i+=8) {
        __m256i out_deg_vec = _mm256_loadu_si256(reinterpret_cast<__m256i*>(&g.out_degree[i]));
        __m256 out_deg_float_vec = _mm256_cvtepi32_ps(out_deg_vec);

        __m256 reciprocal_vec = _mm256_rcp_ps(out_deg_float_vec);

        _mm256_storeu_ps(&inverse_out_degree[i], reciprocal_vec);
    }
    for (; i < n; ++i) {
        inverse_out_degree[i] = 1.0f / (float )g.out_degree[i] ;
    }

    delete [] g.out_degree ;

    const float y = 1.0f/(float )n ;
    const float z = y*(1-d) ;

    __m256 avx_y = _mm256_set1_ps(y);

    __m256 avx_z = _mm256_set1_ps(z);


//#pragma omp parallel for
// TODO : schedule static
    int j = 0 ;
    for(; j < n - 8  ; j+=8) {
        _mm256_store_ps(previousPR+j, avx_y);
    }
    for (; j < n; ++j) {
        previousPR[j] = y ;
    }
    __m256 avx_zeroes = _mm256_set1_ps(0.0f) ;

    __m256 avx_d = _mm256_set1_ps(d);

    for (int i = 0; i < nb_iteration; ++i) {
//#pragma omp parallel for
        int m = 0 ;
        for(; m < n-8; m+=8) {
            _mm256_store_ps(PR + m, avx_zeroes);
        }
        for (; m < n; ++m) {
            PR[m] = 0.0f ;
        }
        // in a fully parallel manner update previous to previous[m] /= inverse_out_degree[m]
        //#pragma omp parallel for
        int s = 0;
        for (; s < n-8; s+=8) {
            __m256 prevPR_vec = _mm256_loadu_ps(&previousPR[s]);
            __m256 inv_out_deg_vec = _mm256_loadu_ps(&inverse_out_degree[s]);

            __m256 result_vec = _mm256_mul_ps(prevPR_vec, inv_out_deg_vec);

            _mm256_storeu_ps(&previousPR[s], result_vec);
        }
        for(;s<m;++s){
            previousPR[s]*=inverse_out_degree[s];
        }

        //TODO : microbenchmark using pair or vector of double size
        //#pragma grainsize 256
        #pragma omp parallel for schedule(dynamic,256)
        for (int l = 0; l < g.src_size ; ++l) {
            // too much random access here
            for (int k = g.src[l].second; k < g.src[l + 1].second ; ++k) {
                #pragma omp atomic
                PR[dst[k]]+= previousPR[g.src[l].first] ;
            }
        }
        //#pragma omp parallel for
        int l = 0 ;
        for(; l < n; l+=8) {
            __m256 avx_PR = _mm256_loadu_ps(&PR[l]);
            avx_PR = _mm256_fmadd_ps(avx_d,avx_PR,avx_z);
            _mm256_storeu_ps(&PR[l],avx_PR) ;
        }
        for (;l < n ; ++l) {
            PR[l] = d * PR[l] + z ;
        }
        // swap PR and previousPR
        temp = previousPR ;
        previousPR = PR ;
        PR  =  temp ;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end- start);
    std::cout << "calculating pageRank took : " << duration.count() << '\n' ;


    std::ofstream out("/home/farouk/CLionProjects/graph-creation/inputs/PageRankOutput.txt");
    for (int j = 0; j < n; ++j) {
        out << previousPR[j]  ;
        out << '\n' ;
    }
    out.close();

    std::sort(previousPR,previousPR+n);

    std::cout << "Top 10 vertices : \n" ;
    for (int i = n-1; i >n-10   ; i--) {
        std::cout << previousPR[i] << '\n' ;
    }
    std::cout << '\n' ;

    std::cout << "sum of al PR = " << std::accumulate(previousPR,previousPR+n,0.0) << '\n';


    delete [] PR ;
    delete [] previousPR ;
}


int main(int argc, char* argv[]) {

    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <filename> <vertices> <nb_iterations> " << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    const int vertices = atoi(argv[2]);
    const int nb_iteration = atoi(argv[3]);

    parallelPageRank(filename,vertices,nb_iteration) ;

    return 0;
}