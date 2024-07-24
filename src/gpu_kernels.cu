#include <cuda_runtime.h>
#include "gpu_kernels.h"

__global__ void blackScholesKernel(const OptionData* options, double* results, int n) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < n) {
        OptionData opt = options[idx];
        double d1 = (log(opt.spot / opt.strike) + (opt.riskFreeRate + 0.5 * opt.volatility * opt.volatility) * opt.maturity) / (opt.volatility * sqrt(opt.maturity));
        double d2 = d1 - opt.volatility * sqrt(opt.maturity);
        results[idx] = opt.spot * normcdf(d1) - opt.strike * exp(-opt.riskFreeRate * opt.maturity) * normcdf(d2);
    }
}

void launchBlackScholesKernel(const OptionData* h_options, double* h_results, int n) {
    OptionData* d_options;
    double* d_results;

    cudaMalloc(&d_options, n * sizeof(OptionData));
    cudaMalloc(&d_results, n * sizeof(double));

    cudaMemcpy(d_options, h_options, n * sizeof(OptionData), cudaMemcpyHostToDevice);

    int blockSize = 256;
    int numBlocks = (n + blockSize - 1) / blockSize;
    blackScholesKernel<<<numBlocks, blockSize>>>(d_options, d_results, n);

    cudaMemcpy(h_results, d_results, n * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_options);
    cudaFree(d_results);
}
