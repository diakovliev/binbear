#include <QDomDocument>
#include <QApplication>
#include <QDomElement>
#include <QDebug>
#include <QVariant>

#include "qbinarydatacolorscheme.h"
#include "qabstractbinarydatasource.h"

QBinaryDataColorScheme::QBinaryDataColorScheme()
    : source_(0)
{
    root_.size          = 0;
    root_.name          = "no name";
    root_.description   = "no descrition";
    root_.color         = Qt::white;
    root_.changedcolor  = Qt::gray;
    root_.editable      = true;
}

void QBinaryDataColorScheme::setDataSource(QAbstractBinaryDataSource *source)
{
    source_ = source;
    if (source_)
    {
        source_->setColorScheme(this);
    }
}

quint64 QBinaryDataColorScheme::readFromDS(quint64 offset, quint64 size) const
{
    quint64 res = 0;

    QModelIndex dataPointer = source_->offsetToIndex(offset);
    if (!dataPointer.isValid())
    {
        qWarning("QBinaryDataColorScheme::readFromDS: Unable to read data from 0x%08x(%ld)", offset, size);
        return res;
    }

    QByteArray data = source_->read(dataPointer, size);

    for (int i = 0; i < data.size(); ++i)
        res |= (data.at(i) & 0xFF) << i;

    return res;
}

QBinaryDataColorScheme::Element
QBinaryDataColorScheme::findElementByIndex(const QModelIndex &index) const
{   
    Element result = root_;

    if (source_ && index.isValid())
    {
        qint64 raw_offset = source_->indexToOffset(index);
        if (raw_offset < 0)
        {
            qWarning("QBinaryDataColorScheme::findElementByIndex: Unable to convert index to offset");
            return result;
        }

        quint64 offset = qAbs(raw_offset);
        quint64 packet_offset = 0;

        // simple case: packets with similar size
        if (root_.size > 0)
        {
            offset = offset % root_.size;
            packet_offset -= offset;
        }
        // hard case: we have to read packet size from input
        else if (root_.size < 0)
        {
            quint64 packet_start = 0;
            quint64 prev_packet_start = 0;
            quint64 packet_size = 0;
            while (packet_start <= offset)
            {
                prev_packet_start = packet_start;
                packet_size = readFromDS(packet_start, qAbs(root_.size));
                packet_start += packet_size + qAbs(root_.size);
            }
            offset = offset - prev_packet_start;
            packet_offset = prev_packet_start;
        }

        // here offset should point to the first byte of needed packet
        quint64 elem_start = 0;

        foreach(Element elem, childs_)
        {
            quint64 elem_end = 0;
            quint64 elem_size = 0;
            if (elem.size > 0)
            {
                elem_end = elem_start + elem.size;
            }
            else if (elem.size < 0)
            {
                if ( (offset >= elem_start) && (offset < (elem_start + qAbs(elem.size))) )
                {
                    // size field, return root
                    return result;
                }
                elem_size = readFromDS(packet_offset + elem_start, qAbs(elem.size));
                elem_end = elem_start + elem_size + qAbs(elem.size);
            }
            else
            {
                // elem_size == 0
                continue;
            }

            if ( (offset >= elem_start) && (offset < elem_end) )
            {
                result = elem;
                break;
            }
            elem_start = elem_end;
        }

    }

    return result;
}

QString QBinaryDataColorScheme::name(const QModelIndex &index) const
{
    Element element = findElementByIndex(index);
    return element.name;
}

QString QBinaryDataColorScheme::descrition(const QModelIndex &index) const
{
    Element element = findElementByIndex(index);
    return element.description;
}

QColor QBinaryDataColorScheme::color(const QModelIndex &index) const
{
    Element element = findElementByIndex(index);
    return element.color;
}

QColor QBinaryDataColorScheme::changedColor(const QModelIndex &index) const
{
    Element element = findElementByIndex(index);
    return element.changedcolor;
}

bool QBinaryDataColorScheme::editable(const QModelIndex &index) const
{
    Element element = findElementByIndex(index);
    return element.editable;
}
