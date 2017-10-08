#include "oit_cuda_kernel.cuh"
#include <thrust/scan.h>
#include <thrust/fill.h>
#include <thrust/device_vector.h>
#include <math_constants.h>
#include <vector_types.h>
#include <helper_math.h>

extern "C"
{
    //int* data: device address of data
    void computePrefixSum(int* dSrcData, int* dDstData, uint count)
    {
        thrust::device_ptr<int> dSrcPtr(dSrcData);
        thrust::device_ptr<int> dDstPtr(dDstData);
        thrust::exclusive_scan(dSrcPtr, dSrcPtr + count, dDstPtr);
    }

    void cudaFill(int *data, int count, int val)
    {
        thrust::device_ptr<int> dPtr(data);
        thrust::fill(dPtr, dPtr + count, (int)val);
    }

}

