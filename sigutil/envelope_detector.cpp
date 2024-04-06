#include "envelope_detector.h"
#include <cmath>

std::vector<datapoint<int, float>> envelope_detection(std::vector<datapoint<int, float>> &signal)
{
    std::vector<datapoint<int, float>> result_envelope;

    // Implement envelope detection here

    float max_value_abs = 0;
    float true_max_value = 0;
    float last_value = 0;

    enum { DOWN, UP } trend;

    for (int i = 0; i < signal.size(); i++)
    {
        float sample = signal[i].y;
        float abs_sample = fabsf(sample);

        if (abs_sample > max_value_abs)
        {
            max_value_abs = abs_sample;
            true_max_value = sample;
        }

        // Zero pass detection. Assumes that values are already averaged.
        if ((last_value > 0 && sample < 0) || (last_value < 0 && sample > 0))
        {
            datapoint<int, float> dp{};
            dp.y = max_value_abs;
            dp.x = i;
            result_envelope.push_back(dp);
            true_max_value = 0;
            max_value_abs = 0;
        }

        last_value = sample;
    }

    return result_envelope;
}
