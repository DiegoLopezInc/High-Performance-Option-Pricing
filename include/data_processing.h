#pragma once
#include <vector>
#include <string>

struct OptionData {
    double strike;
    double maturity;
    double spot;
    double volatility;
    double riskFreeRate;
};

std::vector<OptionData> loadOptionData(const std::string& filename);
void preprocessData(std::vector<OptionData>& options);
void saveProcessedData(const std::vector<OptionData>& options, const std::string& filename);
