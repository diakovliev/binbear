#include <QDebug>

#include "qbinarydataformatter_raw.h"

QBinaryDataFormatter_Raw::QBinaryDataFormatter_Raw(QAbstractBinaryDataSource *source)
    : QAbstractBinaryDataFormatter(source)
{
}

QBinaryDataFormatter_Raw::~QBinaryDataFormatter_Raw()
{
}

/* Format whole data from the source */
QByteArray QBinaryDataFormatter_Raw::format()
{
    return format(source()->index(0,0), QModelIndex());
}

/* Format data range from the source */
QByteArray QBinaryDataFormatter_Raw::format(const QModelIndex &begin, const QModelIndex &end)
{
    QByteArray out;

    if (!begin.isValid())
    {
        qDebug() << "QBinaryDataFormatter_Raw::format: First index should be valid";
        return QByteArray();
    }

    QModelIndex current = begin;
    while (current.isValid())
    {
        QVariant data = source()->data(current, Qt::UserRole);
        if (!data.isNull())
        {
            QMap<QString,QVariant> userData = data.toMap();
            if (userData.contains("char"))
            {
                unsigned char byte = userData["char"].toChar().toLatin1();

                out.append(byte);
            }
            else
            {
                qDebug() << "QBinaryDataFormatter_Raw:: Unable to retrieve user data for index:" << current;
            }
        }
        else
        {
            qDebug() << "QBinaryDataFormatter_Raw:: Unable to retrieve user data for index:" << current;
        }

        if (end.isValid() && current == end)
        {
            break;
        }
        else
        {
            current = source()->nextIndex(current);
        }
    }

    return out;
}

