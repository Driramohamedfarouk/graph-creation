//
// Created by farouk on 11/08/23.
//

#ifndef GRAPH_PROCESSING_FRAMEWORK_E_VECTOR_H
#define GRAPH_PROCESSING_FRAMEWORK_E_VECTOR_H

#include <algorithm>
#include <immintrin.h>

#define GRAIN_SIZE 2048

// a template class that should
// implement methods that populate
// the graph in parallel
// and using c++ intrinsics AVX instructions
template <typename T>
class pv_vector{
private:
    T* begin ;
    T* end ;
    size_t size{} ;
public:
    pv_vector():
    begin(nullptr) ,
    end(nullptr),
    size(0) {}

    // TODO : take care of the alignment here
    explicit pv_vector(size_t size_){
        size_t n = ((size_-1)|7)+1 ;
        size = size_ ;
        begin = static_cast<T*>(_mm_malloc(n * sizeof(T),32));
        end = begin + size_ ;
    }


    ~pv_vector(){
        if(begin!= nullptr)
            _mm_free(begin);
    }

    void fill(T val){
        __m256 avx_val = _mm256_set1_ps(val);
        #pragma omp parallel for schedule(static,GRAIN_SIZE)
        for(uint j = 0 ; j < size  ; j+=8) {
            _mm256_store_ps(begin+j, avx_val);
        }
    }

    void mul_add(T factor, T offset){
        __m256 avx_f = _mm256_set1_ps(factor);
        __m256 avx_o = _mm256_set1_ps(offset);
        #pragma omp parallel for schedule(static,GRAIN_SIZE)
        for(uint l = 0 ; l < size; l+=8) {
            __m256 avx_PR = _mm256_load_ps(begin+l);
            avx_PR = _mm256_fmadd_ps(avx_f,avx_PR,avx_o);
            _mm256_store_ps(begin+l,avx_PR) ;
        }
    }

    // takes a vector of int and construct
    // a parallelized version of its inverses
    void inverse(int * v){
        #pragma omp parallel for schedule(static,GRAIN_SIZE)
        for (uint i = 0; i < size; i+=8) {
            __m256i out_deg_vec = _mm256_load_si256(reinterpret_cast<__m256i*>(v+i));
            __m256 out_deg_float_vec = _mm256_cvtepi32_ps(out_deg_vec);

            __m256 reciprocal_vec = _mm256_rcp_ps(out_deg_float_vec);

            _mm256_store_ps(begin+i, reciprocal_vec);
        }
    }

    T* data(){
        return begin ;
    }


    void static swap(pv_vector<T>& v1 , pv_vector<T>& v2){
        std::swap(v1.begin,v2.begin);
        std::swap(v1.end,v2.end);
        std::swap(v1.size,v2.size);
    }

    // TODO : overload the operator *=
    //  pass by address to avoid copying
    pv_vector<T>& operator*=(pv_vector<T>& other){
        #pragma omp parallel for schedule(static,GRAIN_SIZE)
        for (uint s = 0; s < size; s+=8) {
            __m256 prevPR_vec = _mm256_load_ps(begin+s);
            __m256 inv_out_deg_vec = _mm256_load_ps(other.begin+s);

            __m256 result_vec = _mm256_mul_ps(prevPR_vec, inv_out_deg_vec);

            _mm256_store_ps(begin+s, result_vec);
        }
        return *this ;
    }

    T& operator[](size_t n) {
        return begin[n];
    }

    // this is used when we want to declare a constant pv_vector
    const T& operator[](size_t n) const {
        return begin[n];
    }

};



#endif //GRAPH_PROCESSING_FRAMEWORK_E_VECTOR_H
