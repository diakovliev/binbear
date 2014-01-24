#include "qabstractbinarydataformatter.h"

QAbstractBinaryDataFormatter::QAbstractBinaryDataFormatter(QAbstractBinaryDataSource *source)
    : source_(source)
{
    Q_ASSERT_X(source != 0, "QAbstractBinaryDataFormatter", "Should be valid binary data source");
}

QAbstractBinaryDataFormatter::~QAbstractBinaryDataFormatter()
{
}

QAbstractBinaryDataSource *QAbstractBinaryDataFormatter::source() const
{
    return source_;
}
