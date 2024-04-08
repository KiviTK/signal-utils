#pragma once

#include <vector>
#include "datapoint.h"

/**
 * @brief Generate a sine wave
 *
 * @param frequency The frequency of the sine wave
 * @param amplitude The amplitude of the sine wave
 * @param phase The phase of the sine wave
 * @param sample_rate The sample rate of the sine wave
 * @param duration The duration of the sine wave
 * @return std::vector<datapoint<int, float>> The generated sine wave
 */
std::vector<datapoint<int, float>> generate_sine_wave(float frequency, float amplitude, float phase, float sample_rate, float duration);

/**
 * @brief Generate a cosine wave
 *
 * @param frequency The frequency of the cosine wave
 * @param amplitude The amplitude of the cosine wave
 * @param phase The phase of the cosine wave
 * @param sample_rate The sample rate of the cosine wave
 * @param duration The duration of the cosine wave
 * @return std::vector<datapoint<int, float>> The generated cosine wave
 */
std::vector<datapoint<int, float>> generate_cosine_wave(float frequency, float amplitude, float phase, float sample_rate, float duration);

/**
 * @brief Generate a square wave
 *
 * @param frequency The frequency of the square wave
 * @param amplitude The amplitude of the square wave
 * @param phase The phase of the square wave
 * @param sample_rate The sample rate of the square wave
 * @param fill_factor The fill factor of the square wave
 * @param duration The duration of the square wave
 * @return std::vector<datapoint<int, float>> The generated square wave
 */
std::vector<datapoint<int, float>> generate_square_wave(float frequency, float amplitude, float phase, float sample_rate, float fill_factor, float duration);

/**
 * @brief Generate a sawtooth wave
 *
 * @param frequency The frequency of the sawtooth wave
 * @param amplitude The amplitude of the sawtooth wave
 * @param phase The phase of the sawtooth wave
 * @param sample_rate The sample rate of the sawtooth wave
 * @param duration The duration of the sawtooth wave
 * @return std::vector<datapoint<int, float>> The generated sawtooth wave
 */
std::vector<datapoint<int, float>> generate_sawtooth_wave(float frequency, float amplitude, float phase, float sample_rate, float duration);

/**
 * @brief Generate a triangle wave
 *
 * @param frequency The frequency of the triangle wave
 * @param amplitude The amplitude of the triangle wave
 * @param phase The phase of the triangle wave
 * @param sample_rate The sample rate of the triangle wave
 * @param duration The duration of the triangle wave
 * @return std::vector<datapoint<int, float>> The generated triangle wave
 */
std::vector<datapoint<int, float>> generate_triangle_wave(float frequency, float amplitude, float phase, float sample_rate, float duration);

/**
 * @brief Generate a noise wave
 *
 * @param amplitude The amplitude of the noise wave
 * @param sample_rate The sample rate of the noise wave
 * @param duration The duration of the noise wave
 * @return std::vector<datapoint<int, float>> The generated noise wave
 */
std::vector<datapoint<int, float>> generate_noise_wave(float amplitude, float sample_rate, float duration);

/**
 * @brief Generate a chirp signal
 *
 * @param start_frequency The start frequency of the chirp signal
 * @param end_frequency The end frequency of the chirp signal
 * @param amplitude The amplitude of the chirp signal
 * @param sample_rate The sample rate of the chirp signal
 * @param duration The duration of the chirp signal
 * @return std::vector<datapoint<int, float>> The generated chirp signal
 */
std::vector<datapoint<int, float>> generate_chirp_signal(float start_frequency, float end_frequency, float amplitude, float sample_rate, float duration);

/**
 * @brief Generate a pulse signal

 * @param amplitude The amplitude of the pulse signal
 * @param sample_rate The sample rate of the pulse signal
 * @param duration The duration of the pulse signal
 * @return std::vector<datapoint<int, float>> The generated pulse signal
 */

std::vector<datapoint<int, float>> generate_pulse_signal(float amplitude, float sample_rate, float duration);