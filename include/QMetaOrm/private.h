#pragma once

#ifdef QMETAORM_LIBRARY
#  define QMETAORM_LIBRARY_API __declspec(dllexport)
#else
#  define QMETAORM_LIBRARY_API __declspec(dllimport)
#endif

