#include "qbinarydataformatter_hex.h"

QBinaryDataFormatter_Hex::QBinaryDataFormatter_Hex(QAbstractBinaryDataSource *source)
    : QBinaryDataFormatter_Raw(source)
{
}

QBinaryDataFormatter_Hex::~QBinaryDataFormatter_Hex()
{
}

/* Format data range from the source */
QByteArray QBinaryDataFormatter_Hex::format(const QModelIndex &begin, const QModelIndex &end)
{
    return QBinaryDataFormatter_Raw::format(begin,end).toHex();
}
