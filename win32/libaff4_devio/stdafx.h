// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define __USE_UNIX98

// Windows Header Files:
#include <windows.h>
#include <winsock.h>
#include <winioctl.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <io.h>

// TODO: reference additional headers your program requires here

#if defined( _WIN64 )
typedef __int64			ssize_t;
#else
typedef __int32			ssize_t;
#endif

typedef __int64 off_t_64;

#define ULL_FMT       "%I64u"
#define SLL_FMT       "%I64i"

#include <devio_types.h>

#include <devio.h>

#include <aff4.h>
#include <RDFValue.h>
