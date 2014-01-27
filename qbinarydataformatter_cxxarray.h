#ifndef QBINARYDATAFORMATTER_CXXARRAY_H
#define QBINARYDATAFORMATTER_CXXARRAY_H

#include "qabstractbinarydataformatter.h"

class QBinaryDataFormatter_CxxArray : public QAbstractBinaryDataFormatter
{
public:
    enum { LINE_WIDTH = 10 };

    QBinaryDataFormatter_CxxArray(QAbstractBinaryDataSource *source);
    virtual ~QBinaryDataFormatter_CxxArray();

    /* Format whole data from the source */
    QByteArray format();

    /* Format data range from the source */
    QByteArray format(const QModelIndex &begin, const QModelIndex &end);

private:
    QString name_;

};

#endif // QBINARYDATAFORMATTER_CXXARRAY_H
