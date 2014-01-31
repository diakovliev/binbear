#include <QDebug>

#include "qbinarydatacolorschemasfactory.h"

QBinaryDataColorSchemasFactory::QBinaryDataColorSchemasFactory()
{
}

QBinaryDataColorScheme *QBinaryDataColorSchemasFactory::createColorScheme(QFile &file)
{
    QBinaryDataColorScheme *colorScheme = new QBinaryDataColorScheme();
    QByteArray xmlInput = file.readAll();
    if (!colorScheme->parseScheme(xmlInput))
    {
        qDebug() << "Unable to read color scheme from file:" << file.fileName();
        delete colorScheme;
        colorScheme = 0;
    }
    return colorScheme;
}

QBinaryDataColorScheme *QBinaryDataColorSchemasFactory::createColorScheme(const QByteArray &xmlInput)
{
    QBinaryDataColorScheme *colorScheme = new QBinaryDataColorScheme();
    if (!colorScheme->parseScheme(xmlInput))
    {
        delete colorScheme;
        colorScheme = 0;
    }
    return colorScheme;
}

QList<QBinaryDataColorScheme*> QBinaryDataColorSchemasFactory::createColorSchemas(QDir &dir)
{
    QList<QBinaryDataColorScheme*> result;

    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    //dir.setSorting(QDir::Size | QDir::Reversed);

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
//        else if(fileInfo.isDir())
//        {
//            QDir recDir(fileInfo.absoluteFilePath());
//            result.append(createColorSchemas(recDir));
//        }
    }

    return result;
}
