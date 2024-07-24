#include <cmath>
#include <random>
#include "option_pricing.h"

double BlackScholesCall(const OptionData& option) {
    double d1 = (std::log(option.spot / option.strike) + (option.riskFreeRate + 0.5 * std::pow(option.volatility, 2)) * option.maturity) / (option.volatility * std::sqrt(option.maturity));
    double d2 = d1 - option.volatility * std::sqrt(option.maturity);
    
    return option.spot * std::erfc(-d1 / std::sqrt(2)) / 2 - option.strike * std::exp(-option.riskFreeRate * option.maturity) * std::erfc(-d2 / std::sqrt(2)) / 2;
}

double BinomialTreeCall(const OptionData& option, int steps) {
    double dt = option.maturity / steps;
    double u = std::exp(option.volatility * std::sqrt(dt));
    double d = 1 / u;
    double p = (std::exp(option.riskFreeRate * dt) - d) / (u - d);

    std::vector<double> prices(steps + 1);
    for (int i = 0; i <= steps; ++i) {
        prices[i] = option.strike * std::pow(u, steps - i) * std::pow(d, i);
    }

    for (int step = steps - 1; step >= 0; --step) {
        for (int i = 0; i <= step; ++i) {
            prices[i] = std::exp(-option.riskFreeRate * dt) * (p * prices[i] + (1 - p) * prices[i + 1]);
        }
    }

    return prices[0];
}

double MonteCarloCall(const OptionData& option, int simulations) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0, 1);

    double sum = 0.0;
    for (int i = 0; i < simulations; ++i) {
        double z = d(gen);
        double ST = option.spot * std::exp((option.riskFreeRate - 0.5 * std::pow(option.volatility, 2)) * option.maturity + option.volatility * std::sqrt(option.maturity) * z);
        sum += std::max(ST - option.strike, 0.0);
    }

    return std::exp(-option.riskFreeRate * option.maturity) * sum / simulations;
}
