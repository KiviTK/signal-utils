#pragma once

#include <vector>

#include "datapoint.h"


struct demod_datapoint
{
    float value;
    int original_index;
};


std::vector<datapoint<int, float>> envelope_detection(std::vector<datapoint<int, float>>& signal);