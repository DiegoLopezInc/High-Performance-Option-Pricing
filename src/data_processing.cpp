#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include "data_processing.h"

struct OptionData {
    double strike;
    double maturity;
    double spot;
    double volatility;
    double riskFreeRate;
};

std::vector<OptionData> loadOptionData(const std::string& filename) {
    std::vector<OptionData> options;
    std::ifstream file(filename);
    std::string line;

    // Skip header
    std::getline(file, line);

    while (std::getline(file, line)) {
        OptionData option;
        sscanf(line.c_str(), "%lf,%lf,%lf,%lf,%lf", &option.strike, &option.maturity,
               &option.spot, &option.volatility, &option.riskFreeRate);
        options.push_back(option);
    }

    return options;
}

void preprocessData(std::vector<OptionData>& options) {
    // Remove outliers (simple z-score method)
    double mean = std::accumulate(options.begin(), options.end(), 0.0,
        [](double sum, const OptionData& opt) { return sum + opt.volatility; }) / options.size();
    
    double sq_sum = std::inner_product(options.begin(), options.end(), options.begin(), 0.0,
        std::plus<>(), [mean](const OptionData& a, const OptionData& b) {
            return std::pow(a.volatility - mean, 2);
        });
    double stdev = std::sqrt(sq_sum / (options.size() - 1));

    options.erase(std::remove_if(options.begin(), options.end(),
        [mean, stdev](const OptionData& opt) {
            return std::abs(opt.volatility - mean) > 3 * stdev;
        }), options.end());

    // Normalize data
    for (auto& opt : options) {
        opt.strike /= opt.spot;
        opt.maturity = std::max(opt.maturity, 1e-6);  // Avoid division by zero
    }
}

void saveProcessedData(const std::vector<OptionData>& options, const std::string& filename) {
    std::ofstream file(filename);
    file << "Strike,Maturity,Spot,Volatility,RiskFreeRate\n";
    for (const auto& opt : options) {
        file << opt.strike << "," << opt.maturity << "," << opt.spot << ","
             << opt.volatility << "," << opt.riskFreeRate << "\n";
    }
}