#include "generator.h"

#include <cmath>

std::vector<datapoint<int, float>>
generate_sine_wave(float frequency, float amplitude, float phase, float sample_rate, float duration)
{
    std::vector<datapoint<int, float>> sine_wave;
    for (int i = 0; i < duration * sample_rate; i++)
    {
        float t = ((float)i) / sample_rate;
        sine_wave.push_back(
                {i,
                 static_cast<float>(amplitude * sin(2 * M_PI * frequency * t + phase))});
    }
    return sine_wave;
}

std::vector<datapoint<int, float>>
generate_cosine_wave(float frequency, float amplitude, float phase, float sample_rate, float duration)
{
    std::vector<datapoint<int, float>> cosine_wave;
    for (int i = 0; i < duration * sample_rate; i++)
    {
        float t = ((float)i) / sample_rate;
        cosine_wave.push_back(
                {i,
                 static_cast<float>(amplitude * cos(2 * M_PI * frequency * t + phase))});
    }
    return cosine_wave;
}

std::vector<datapoint<int, float>>
generate_square_wave(float frequency, float amplitude, float phase, float sample_rate, float fill_factor, float duration)
{
    std::vector<datapoint<int, float>> square_wave;
    for (int i = 0; i < duration * sample_rate; i++)
    {
        float t = ((float)i) / sample_rate;
        square_wave.push_back(
                {i,
                 static_cast<float>(amplitude * (sin(2 * M_PI * frequency * t + phase) > 0 ? 1 : -1))});
    }
    return square_wave;
}

std::vector<datapoint<int, float>>
generate_sawtooth_wave(float frequency, float amplitude, float phase, float sample_rate, float duration)
{
    std::vector<datapoint<int, float>> sawtooth_wave;
    for (int i = 0; i < duration * sample_rate; i++)
    {
        float t = ((float)i) / sample_rate;
        sawtooth_wave.push_back(
                {i,
                 static_cast<float>(amplitude * (2 * (t * frequency - floor(t * frequency)) - 1))});
    }
    return sawtooth_wave;
}

std::vector<datapoint<int, float>>
generate_triangle_wave(float frequency, float amplitude, float phase, float sample_rate, float duration)
{
    std::vector<datapoint<int, float>> triangle_wave;
    for (int i = 0; i < duration * sample_rate; i++)
    {
        float t = ((float)i) / sample_rate;
        triangle_wave.push_back(
                {i,
                 static_cast<float>(amplitude * (2 * fabs(2 * (t * frequency - floor(t * frequency)) - 1) - 1))});
    }
    return triangle_wave;
}

std::vector<datapoint<int, float>> generate_noise_wave(float amplitude, float sample_rate, float duration)
{
    std::vector<datapoint<int, float>> noise_wave;
    for (int i = 0; i < duration * sample_rate; i++)
    {
        noise_wave.push_back(
                {i,
                 static_cast<float>(amplitude * (2 * static_cast<float>(rand()) / RAND_MAX - 1))});
    }
    return noise_wave;
}

std::vector<datapoint<int, float>>
generate_chirp_signal(float start_frequency, float end_frequency, float amplitude, float sample_rate, float duration)
{
    std::vector<datapoint<int, float>> chirp_signal;
    for (int i = 0; i < duration * sample_rate; i++)
    {
        float t = ((float)i) / sample_rate;
        chirp_signal.push_back(
                {i,
                 static_cast<float>(amplitude * sin(2 * M_PI * (start_frequency * t + (end_frequency - start_frequency) / 2 * t * t) + 0))});
    }
    return chirp_signal;
}

std::vector<datapoint<int, float>> generate_pulse_signal(float amplitude, float sample_rate, float duration)
{
    std::vector<datapoint<int, float>> pulse_signal;
    pulse_signal.reserve(duration * sample_rate);

    pulse_signal.push_back({0, amplitude});

    for (int i = 1; i < duration * sample_rate; i++)
    {
        pulse_signal.push_back({i, 0.0f});
    }

    return pulse_signal;
}
