#ifndef SIGNALPROCESSING_HPP
#define SIGNALPROCESSING_HPP
/**
 * @file signalprocessing.hpp
 * @author Raymon Albricht, Tristan Bosveld
 * @brief
 * @version 0.1
 * @date 2023-30-05
 *
 * @copyright Fully open-source
 *
 */

namespace dsp
{
    struct signal_point
    {
        int frequency;
        float amplitude;
    };


void remove_dc(std::vector<float> &vec);

void hamming_windows(std::vector<float> &vec);

void custom_logarithm_abs(std::vector<float> &vec);

void fouriertransf(std::vector<float>& vec);

void inversefouriertransf(std::vector<float>& vec);

signal_point get_voice_peak(
    std::vector<float> audio_data,
    const unsigned int &frequency_floor,
    const unsigned int &frequency_ceiling,
    const int &sample_rate);

signal_point get_peak(const std::vector<float> &cepstrum, const size_t &floor, const size_t &ceiling, const int &sample_rate);

}

#endif //SIGNALPROCESSING_HPP