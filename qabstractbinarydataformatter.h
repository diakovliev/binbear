#ifndef QABSTRACTBINARYDATAFORMATTER_H
#define QABSTRACTBINARYDATAFORMATTER_H

#include <QByteArray>

#include "qabstractbinarydatasource.h"

class QAbstractBinaryDataFormatter
{
public:
    QAbstractBinaryDataFormatter(QAbstractBinaryDataSource *source);
    virtual ~QAbstractBinaryDataFormatter();

    /* Format whole data from the source */
    virtual QByteArray format() = 0;

    /* Format data range from the source */
    virtual QByteArray format(const QModelIndex &begin, const QModelIndex &end) = 0;

    QAbstractBinaryDataSource *source() const;

private:
    QAbstractBinaryDataSource *source_;

};

#endif // QABSTRACTBINARYDATAFORMATTER_H
