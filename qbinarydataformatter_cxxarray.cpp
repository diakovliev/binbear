#include <QDebug>

#include "qbinarydataformatter_cxxarray.h"

QBinaryDataFormatter_CxxArray::QBinaryDataFormatter_CxxArray(const QString &name, QAbstractBinaryDataSource *source)
    : QAbstractBinaryDataFormatter(source)
    , name_(name)
{   
}

QBinaryDataFormatter_CxxArray::~QBinaryDataFormatter_CxxArray()
{
}

/* Format whole data from the source */
QByteArray QBinaryDataFormatter_CxxArray::format()
{
    return format(source()->index(0,0), QModelIndex());
}

/* Format data range from the source */
QByteArray QBinaryDataFormatter_CxxArray::format(const QModelIndex &begin, const QModelIndex &end)
{
    QString out;
    unsigned int line_width_counter = 0;

    if (!begin.isValid())
    {
        qDebug() << "QBinaryDataFormatter_CxxArray::format: First index should be valid";
        return QByteArray();
    }

    /* Header */
    out += QString("unsigned char %1[] = {\n").arg(name_);

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

                out += QString().sprintf(" 0x%02X", byte);
            }
            else
            {
                qDebug() << "QBinaryDataFormatter_CxxArray:: Unable to retrieve user data for index:" << current;

                out += QString().sprintf(" error");
            }
        }
        else
        {
            qDebug() << "QBinaryDataFormatter_CxxArray:: Unable to retrieve user data for index:" << current;

            out += QString().sprintf(" error");
        }

        if (end.isValid() && current == end)
        {
            break;
        }
        else
        {
            current = source()->nextIndex(current);
        }

        ++line_width_counter;

        if (line_width_counter >= LINE_WIDTH)
        {
            out += QString(",\n");

            line_width_counter = 0;
        }
        else
        {
            out += QString(",");
        }
    }

    out += QString("\n}; /* %1 */\n").arg(name_);

    return out.toAscii();
}
