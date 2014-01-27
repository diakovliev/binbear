#ifndef QBINARYDATAFORMATSFACTORY_H
#define QBINARYDATAFORMATSFACTORY_H

#include "qabstractbinarydatasource.h"
#include "qabstractbinarydataformatter.h"

class QBinaryDataFormatsFactory
{
public:
    QBinaryDataFormatsFactory();

    enum Formatters {
        Raw,
        Cxx,
        Base64,
        Hex
    };

    static QAbstractBinaryDataFormatter *createFormatter(Formatters formatter, QAbstractBinaryDataSource *source);

    static QMap<Formatters, QString> supportedFormats();

};

#endif // QBINARYDATAFORMATSFACTORY_H
