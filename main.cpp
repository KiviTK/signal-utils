#include <iostream>
#include <cmath>
#include <fstream>
#include <chrono>

#include "sigutil/envelope_detector.h"
#include "sigutil/finite_impuse_response_filter.h"
#include "sigutil/generator.h"

void am_test()
{
    auto start = std::chrono::high_resolution_clock::now();

    printf("Running envelope detector test\n");


    float fc = 5000;
    float fs = 100;
    std::ofstream signal_out("amsignal.txt");
    std::ofstream demod_out("demod.txt");


    std::vector<datapoint<int, float>> am_signal;
    for (int i = 0; i < 20000; i++)
    {
        float t = ((float)i)*.00001;
        am_signal.push_back(
                {i,
                 static_cast<float>(sin(t * fc) * (0.5 + (0.15*((sin(t * fs))))))});

        //am_signal.push_back(sin(t * fc + 0.3) * sin(t * fs));
    }

    for (auto modulated_signal : am_signal)
    {
        signal_out << modulated_signal.y << '\n';
    }

    signal_out.close();

    auto demod = envelope_detection(am_signal);

    for (auto demod_sample : demod)
    {
        demod_out << demod_sample.y << ' ' << demod_sample.x << '\n';
    }

    demod_out.close();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    printf("Envelope detector test done! Execution time: %lld ms\n\n", duration);
}



void fir_test()
{
    auto start = std::chrono::high_resolution_clock::now();

    printf("Running FIR test\n");

    // Create a simple low pass filter
    std::vector<float> coeffs = {0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05};

    continuous_fir_filter filter(coeffs);

    std::ofstream signal_out("signal.txt");
    std::ofstream filtered_out("filtered.txt");

    std::vector<datapoint<int, float>> signal = generate_square_wave(0.3, 1, 0, 100, 0.5, 10);
    std::vector<datapoint<int, float>> noise = generate_sine_wave(30, 0.3, 0, 100, 10);

    // Combine signal and noise
    for (int i = 0; i < signal.size(); i++)
    {
        signal[i].y += noise[i].y;
    }

    for (auto sample : signal)
    {
        signal_out << sample.y << '\n';
    }

    signal_out.close();

    for (auto sample : signal)
    {
        filter << sample.y;
        float filtered_sample;
        filter >> filtered_sample;
        filtered_out << filtered_sample << '\n';
    }

    filtered_out.close();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    printf("FIR test done! Execution time: %lld ms\n\n", duration);
}

int main()
{
    // Run the tests
    fir_test();
    am_test();
}
