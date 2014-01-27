#include "qbinarydataformatter_base64.h"

QBinaryDataFormatter_Base64::QBinaryDataFormatter_Base64(QAbstractBinaryDataSource *source)
    : QBinaryDataFormatter_Raw(source)
{
}

QBinaryDataFormatter_Base64::~QBinaryDataFormatter_Base64()
{
}

/* Format data range from the source */
QByteArray QBinaryDataFormatter_Base64::format(const QModelIndex &begin, const QModelIndex &end)
{
    return QBinaryDataFormatter_Raw::format(begin,end).toBase64();
}
