#ifndef QBINARYDATACOLORSCHEMASFACTORY_H
#define QBINARYDATACOLORSCHEMASFACTORY_H

#include <QFile>
#include <QDir>

#include "qbinarydatacolorscheme.h"

class QBinaryDataColorSchemasFactory
{
public:
    QBinaryDataColorSchemasFactory();

    static QBinaryDataColorScheme *createColorScheme(QFile &file);
    static QBinaryDataColorScheme *createColorScheme(const QByteArray &xmlInput);

    static QList<QBinaryDataColorScheme*> createColorSchemas(QDir &dir);

};

#endif // QBINARYDATACOLORSCHEMASFACTORY_H
