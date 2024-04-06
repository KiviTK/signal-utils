#include "finite_impuse_response_filter.h"

void continuous_fir_filter::reset_filter()
{
    sample_memory.clear();
    sample_memory.resize(coeffs.size(), 0.0f);
}

float continuous_fir_filter::get_latest_value()
{
    // Perform the convolution
    float result = 0;
    for (size_t i = 0; i < coeffs.size(); i++)
    {
        result += coeffs[i] * sample_memory[get_transformed_index(i)];
    }

    return result;
}


size_t continuous_fir_filter::get_transformed_index(size_t index)
{
    return (currently_newest_sample + index) % sample_memory.size();
}

void continuous_fir_filter::operator<<(float value)
{
    add_new_input_value(value);
}

void continuous_fir_filter::operator>>(float &value)
{
    value = get_latest_value();
}

void continuous_fir_filter::add_new_input_value(float value)
{
    sample_memory[currently_newest_sample] = value;
    currently_newest_sample = (currently_newest_sample + 1) % sample_memory.size();
}
