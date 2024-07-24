#pragma once
#include "data_processing.h"

double BlackScholesCall(const OptionData& option);
double BinomialTreeCall(const OptionData& option, int steps);
double MonteCarloCall(const OptionData& option, int simulations);
