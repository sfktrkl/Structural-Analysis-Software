#ifndef SASSTATICLIBRARY_GLOBAL_H
#define SASSTATICLIBRARY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SASSTATICLIBRARY_LIBRARY)
#  define SASSTATICLIBRARYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SASSTATICLIBRARYSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SASSTATICLIBRARY_GLOBAL_H
