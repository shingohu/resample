/* resample.h - sampling rate conversion subroutines
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

#define FP_DIGITS 15
#define FP_FACTOR (1 << FP_DIGITS)
#define FP_MASK (FP_FACTOR - 1)

#define MAX_HWORD (32767)
#define MIN_HWORD (-32768)

#ifndef MAX
#define MAX(x, y) ((x)>(y) ?(x):(y))
#endif
#ifndef MIN
#define MIN(x, y) ((x)<(y) ?(x):(y))
#endif


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if _WIN32
#include <windows.h>
#else

#include <pthread.h>
#include <unistd.h>

#endif

#if _WIN32
#define FFI_PLUGIN_EXPORT __declspec(dllexport)
#else
#define FFI_PLUGIN_EXPORT
#endif

FFI_PLUGIN_EXPORT uint64_t
resample_s16(const int16_t *input, int16_t *output, int inSampleRate, int outSampleRate,
             uint64_t inputSize,
             int channels);
