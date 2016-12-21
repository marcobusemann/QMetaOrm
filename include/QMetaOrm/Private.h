#pragma once

#ifdef Q_OS_WIN
#  define QMETAORM_DECL_EXPORT     __declspec(dllexport)
#  define QMETAORM_DECL_IMPORT     __declspec(dllimport)
#elif defined(QT_VISIBILITY_AVAILABLE)
#  define QMETAORM_DECL_EXPORT     __attribute__((visibility("default")))
#  define QMETAORM_DECL_IMPORT     __attribute__((visibility("default")))
#else
#  define QMETAORM_DECL_EXPORT
#  define QMETAORM_DECL_IMPORT
#endif

#ifdef QMETAORM_LIBRARY
#  define QMETAORM_LIBRARY_API QMETAORM_DECL_EXPORT
#else
#  define QMETAORM_LIBRARY_API QMETAORM_DECL_IMPORT
#endif

