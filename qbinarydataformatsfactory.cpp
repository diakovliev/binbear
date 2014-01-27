#include <QDebug>

#include "qbinarydataformatsfactory.h"

#include "qbinarydataformatter_raw.h"
#include "qbinarydataformatter_base64.h"
#include "qbinarydataformatter_hex.h"
#include "qbinarydataformatter_cxxarray.h"

QBinaryDataFormatsFactory::QBinaryDataFormatsFactory()
{
}

QAbstractBinaryDataFormatter *QBinaryDataFormatsFactory::createFormatter(Formatters formatter, QAbstractBinaryDataSource *source)
{
    QAbstractBinaryDataFormatter *res = 0;

    switch (formatter)
    {
    case Raw:
        res = new QBinaryDataFormatter_Raw(source);
    break;
    case Cxx:
        res = new QBinaryDataFormatter_CxxArray(source);
    break;
    case Base64:
        res = new QBinaryDataFormatter_Base64(source);
    break;
    case Hex:
        res = new QBinaryDataFormatter_Hex(source);
    break;
    default:
        qDebug() << "QBinaryDataFormatsFactory::createFormatter: Unsupported format id:" << formatter;
        ;
    }

    return res;
}

QMap<QBinaryDataFormatsFactory::Formatters, QString> QBinaryDataFormatsFactory::supportedFormats()
{
    QMap<QBinaryDataFormatsFactory::Formatters, QString> res;

    //res.insert(Raw,     QObject::tr("Raw binary data"));
    res.insert(Cxx,     QObject::tr("Binary data formatted as c/c++ array"));
    res.insert(Base64,  QObject::tr("Binary data encoded as base64 data"));
    res.insert(Hex,     QObject::tr("Binary data encoded as hex string"));

    return res;
}
