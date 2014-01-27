#ifndef QBINARYDATAFORMATTER_BASE64_H
#define QBINARYDATAFORMATTER_BASE64_H

#include "qbinarydataformatter_raw.h"

class QBinaryDataFormatter_Base64 : public QBinaryDataFormatter_Raw
{
public:
    QBinaryDataFormatter_Base64(QAbstractBinaryDataSource *source);
    virtual ~QBinaryDataFormatter_Base64();

    /* Format data range from the source */
    QByteArray format(const QModelIndex &begin, const QModelIndex &end);
        
};

#endif // QBINARYDATAFORMATTER_BASE64_H
