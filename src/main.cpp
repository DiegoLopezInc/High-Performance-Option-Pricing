#include <iostream>
#include <chrono>
#include <vector>
#include "data_processing.h"
#include "option_pricing.h"
#include "gpu_kernels.h"

int main() {
    // Load and preprocess data
    std::vector<OptionData> options = loadOptionData("data/option_data.csv");
    preprocessData(options);

    // CPU pricing
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<double> cpuResults;
    for (const auto& opt : options) {
        cpuResults.push_back(BlackScholesCall(opt));
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> cpuDuration = end - start;

    // GPU pricing
    start = std::chrono::high_resolution_clock::now();
    std::vector<double> gpuResults(options.size());
    launchBlackScholesKernel(options.data(), gpuResults.data(), options.size());
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> gpuDuration = end - start;

    // Compare results and performance
    double maxDiff = 0.0;
    for (size_t i = 0; i < options.size(); ++i) {
        maxDiff = std::max(maxDiff, std::abs(cpuResults[i] - gpuResults[i]));
    }

    std::cout << "CPU time: " << cpuDuration.count() << " seconds\n";
    std::cout << "GPU time: " << gpuDuration.count() << " seconds\n";
    std::cout << "Max difference: " << maxDiff << "\n";

    return 0;
}
