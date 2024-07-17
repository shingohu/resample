/* resample.c - sampling rate conversion subroutines
 *
 * Original version available at the 
 * Digital Audio Resampling Home Page located at
 * http://ccrma.stanford.edu/~jos/resample/.
 *
 * Modified for use on Android by Ethan Chen
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "resample.h"

uint64_t resample_s16(const int16_t *input, int16_t *output, int inSampleRate, int outSampleRate,
                      uint64_t inputSize,
                      int channels
) {
    if (input == NULL)
        return 0;
    uint64_t outputSize = (uint64_t)(inputSize * (double) outSampleRate / (double) inSampleRate);
    outputSize -= outputSize % channels;
    if (output == NULL)
        return outputSize;
    double stepDist = ((double) inSampleRate / (double) outSampleRate);
    const uint64_t fixedFraction = (1LL << 32);
    const double normFixed = (1.0 / (1LL << 32));
    uint64_t step = ((uint64_t)(stepDist * fixedFraction + 0.5));
    uint64_t curOffset = 0;
    for (uint32_t i = 0; i < outputSize; i += 1) {
        for (uint32_t c = 0; c < channels; c += 1) {
            *output++ = (int16_t)(input[c] + (input[c + channels] - input[c]) * (
                                          (double) (curOffset >> 32) + ((curOffset & (fixedFraction - 1)) * normFixed)
                                  )
            );
        }
        curOffset += step;
        input += (curOffset >> 32) * channels;
        curOffset &= (fixedFraction - 1);
    }
    return outputSize;
}

