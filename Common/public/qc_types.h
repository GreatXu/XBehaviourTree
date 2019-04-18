//
//  qc_types.h
//  qqdz
//
//  Created by ldm on 2017/4/17.
//  Copyright © 2017年 ldm. All rights reserved.
//

#ifndef qc_types_h
#define qc_types_h

/* Use the real stdint.h if it's there (taken from Paul Hsieh's pstdint.h) */
#if (defined(__STDC__) && __STDC__ && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || (defined(__GNUC__) && (defined(_STDINT_H) || defined(_STDINT_H_)) || defined (HAVE_STDINT_H))
#include <stdint.h>

typedef int16_t qint16;
typedef uint16_t quint16;
typedef int32_t qint32;
typedef uint32_t quint32;
#elif defined(_WIN32)

#  if defined(__CYGWIN__)
#    include <_G_config.h>
typedef _G_int32_t qint32;
typedef _G_uint32_t quint32;
typedef _G_int16 qint16;
typedef _G_uint16 quint16;
#  elif defined(__MINGW32__)
typedef short qint16;
typedef unsigned short quint16;
typedef int qint32;
typedef unsigned int quint32;
#  elif defined(__MWERKS__)
typedef int qint32;
typedef unsigned int quint32;
typedef short qint16;
typedef unsigned short quint16;
#  else
/* MSVC/Borland */
typedef __int32 qint32;
typedef unsigned __int32 quint32;
typedef __int16 qint16;
typedef unsigned __int16 quint16;
#  endif

#elif defined(__MACOS__)

#  include <sys/types.h>
typedef SInt16 qint16;
typedef UInt16 quint16;
typedef SInt32 qint32;
typedef UInt32 quint32;

#elif (defined(__APPLE__) && defined(__MACH__)) /* MacOS X Framework build */

#  include <sys/types.h>
typedef int16_t qint16;
typedef u_int16_t quint16;
typedef int32_t qint32;
typedef u_int32_t quint32;

#elif defined(__BEOS__)

/* Be */
#  include <inttypes.h>
typedef int16 qint16;
typedef u_int16 quint16;
typedef int32_t qint32;
typedef u_int32_t quint32;

#elif defined (__EMX__)

/* OS/2 GCC */
typedef short qint16;
typedef unsigned short quint16;
typedef int qint32;
typedef unsigned int quint32;

#elif defined (DJGPP)

/* DJGPP */
typedef short qint16;
typedef unsigned short quint16;
typedef int qint32;
typedef unsigned int quint32;

#elif defined(R5900)

/* PS2 EE */
typedef int qint32;
typedef unsigned quint32;
typedef short qint16;
typedef unsigned short quint16;

#elif defined(__SYMBIAN32__)

/* Symbian GCC */
typedef signed short qint16;
typedef unsigned short quint16;
typedef signed int qint32;
typedef unsigned int quint32;

#elif defined(CONFIG_TI_C54X) || defined (CONFIG_TI_C55X)

typedef short qint16;
typedef unsigned short quint16;
typedef long qint32;
typedef unsigned long quint32;

#elif defined(CONFIG_TI_C6X)

typedef short qint16;
typedef unsigned short quint16;
typedef int qint32;
typedef unsigned int quint32;

#else

/* Give up, take a reasonable guess */
typedef short qint16;
typedef unsigned short quint16;
typedef int qint32;
typedef unsigned int quint32;

#endif

#define qint         int                     /* used for counters etc; at least 16 bits */
#define qint64       long long
#define qint8        signed char

#define quint        unsigned int            /* used for counters etc; at least 16 bits */
#define quint64      unsigned long long
#define quint8       unsigned char

#endif