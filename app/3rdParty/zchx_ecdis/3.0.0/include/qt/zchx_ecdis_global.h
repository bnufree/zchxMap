#ifndef ZCHX_ECDIS_GLOBAL_H_
#define ZCHX_ECDIS_GLOBAL_H_
#include <qglobal.h>
#include <QLoggingCategory>

#if defined(ZCHX_ECDIS_PLUGIN)
#  define ZCHX_ECDIS_LIB Q_DECL_EXPORT
#else
#  define ZCHX_ECDIS_LIB Q_DECL_IMPORT
#endif

Q_DECLARE_LOGGING_CATEGORY(ecdis)

#endif
