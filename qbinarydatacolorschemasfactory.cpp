#include <QDebug>

#include "qbinarydatacolorschemasfactory.h"
#include "qbinarydatacolorxmlschemeparser.h"

QBinaryDataColorSchemasFactory::QBinaryDataColorSchemasFactory()
{
}

QBinaryDataColorScheme *QBinaryDataColorSchemasFactory::createColorScheme(QFile &file)
{
    QBinaryDataColorXmlSchemeParser parser;
    return parser.parseColorScheme(file.readAll());
}

QBinaryDataColorScheme *QBinaryDataColorSchemasFactory::createColorScheme(const QByteArray &xmlInput)
{
    QBinaryDataColorXmlSchemeParser parser;
    return parser.parseColorScheme(xmlInput);
}

QList<QBinaryDataColorScheme*> QBinaryDataColorSchemasFactory::createColorSchemas(QDir &dir)
{
    QList<QBinaryDataColorScheme*> result;

    dir.setFilter(QDir::Files | QDir::NoSymLinks);

    QFileInfoList list = dir.entryInfoList();
    foreach (QFileInfo fileInfo, list)
    {
        if (fileInfo.isFile())
        {
            QFile file(fileInfo.absoluteFilePath());
            if (!file.open(QIODevice::ReadOnly))
            {
                qDebug() << "Unable to open file:" << file.fileName();
                continue;
            }

            QBinaryDataColorScheme *colorScheme = createColorScheme(file);
            if (colorScheme)
            {
                result.append(colorScheme);
            }
        }
    }

    return result;
}
