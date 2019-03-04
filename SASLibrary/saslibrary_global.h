#pragma once

#include <QtCore/qglobal.h>

#if defined(SASLIBRARY_LIBRARY)
#  define SASLIBRARYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SASLIBRARYSHARED_EXPORT Q_DECL_IMPORT
#endif

#define MAKE_TESTABLE(className) friend class TestCase

