#pragma once

#include <vector>

#include "datapoint.h"

class continuous_fir_filter
{
public:
    explicit continuous_fir_filter(std::vector<float> coeffs) : coeffs(coeffs)
    {
        sample_memory.resize(coeffs.size(), 0.0f);
    }
    
    float get_latest_value();
    void add_new_input_value(float value);

    void operator<<(float value);
    void operator>>(float& value);

    void reset_filter();

    std::vector<float> coeffs;

private:
    size_t currently_newest_sample = 0;

    /**
     * @brief Get the transformed index of the sample memory.
     * This implements the indexing for the ring buffer
     *
     * @param index The index of the sample memory.
     * @return size_t The transformed index.
     */
    size_t get_transformed_index(size_t index);

    /**
     * @brief The memory of the filter. This is where the filter stores the last N samples.
     *
     * Acts as a ring buffer.
     * */
    std::vector<float> sample_memory;
};