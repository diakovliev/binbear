#ifndef QBINARYDATAFORMATTER_HEX_H
#define QBINARYDATAFORMATTER_HEX_H

#include "qbinarydataformatter_raw.h"

class QBinaryDataFormatter_Hex : public QBinaryDataFormatter_Raw
{
public:
    QBinaryDataFormatter_Hex(QAbstractBinaryDataSource *source);
    virtual ~QBinaryDataFormatter_Hex();

    /* Format data range from the source */
    QByteArray format(const QModelIndex &begin, const QModelIndex &end);
};

#endif // QBINARYDATAFORMATTER_HEX_H
