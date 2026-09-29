// SPDX-FileCopyrightText: 2002-2023 PCSX2 Dev Team
// SPDX-License-Identifier: LGPL-3.0+

// Includes appropriate intrinsic header based on platform.

#pragma once

#ifdef _MSC_VER
#include <intrin.h>
#endif

#if defined(_M_X86)

#if defined(__AVX2__)
#define _M_SSE 0x501
#elif defined(__AVX__)
#define _M_SSE 0x500
#elif defined(__SSE4_1__)
#define _M_SSE 0x401
#else
#error PCSX2 requires compiling for at least SSE 4.1
#endif

// Starting with AVX, processors have fast unaligned loads
// Reduce code duplication by not compiling multiple versions
#if _M_SSE >= 0x500
#define FAST_UNALIGNED 1
#else
#define FAST_UNALIGNED 0
#endif

#include <xmmintrin.h>
#include <emmintrin.h>
#include <tmmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>

#elif defined(__aarch64__) && !defined(_MSC_VER)

// GSVector is written against SSE intrinsics; sse2neon maps them onto NEON.
// _M_SSE stays undefined (0), matching the SSE4.1 baseline the Windows build ships.
#define FAST_UNALIGNED 0
#include "sse2neon.h"

#elif defined(_M_ARM64)
#if defined(_MSC_VER) && !defined(__clang__)
#include <arm64_neon.h>
#else
#include <arm_neon.h>
#endif
#endif

#ifdef __APPLE__
#include <stdlib.h> // alloca
#else
#include <malloc.h> // alloca
#endif
