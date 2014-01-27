#ifndef QBINARYDATAFORMATTER_RAW_H
#define QBINARYDATAFORMATTER_RAW_H

#include "qabstractbinarydataformatter.h"

class QBinaryDataFormatter_Raw : public QAbstractBinaryDataFormatter
{
public:
    QBinaryDataFormatter_Raw(QAbstractBinaryDataSource *source);
    virtual ~QBinaryDataFormatter_Raw();

    /* Format whole data from the source */
    virtual QByteArray format();

    /* Format data range from the source */
    virtual QByteArray format(const QModelIndex &begin, const QModelIndex &end);

};

#endif // QBINARYDATAFORMATTER_RAW_H
