#include <QDomDocument>
#include <QApplication>
#include <QDomElement>
#include <QDebug>
#include <QVariant>


#include "qbinarydatacolorscheme.h"
#include "qabstractbinarydatasource.h"

#if 0
#define TEST_SCHEME \
    "<color_scheme>" \
    "<size>188</size>" \
    "<name>PES</name>" \
    "<description>descrition</description>" \
    "<color>white</color>" \
    "<changedcolor>gray</changedcolor>" \
    "<editable>false</editable>" \    
    "<element>" \
    "   <size>2</size>" \
    "   <name>First int</name>" \
    "   <description>desc 1</description>" \
    "   <color>red</color>" \
    "   <changedcolor>magenta</changedcolor>" \
    "   <editable>true</editable>" \
    "</element>" \
    "<element>" \
    "   <size>2</size>" \
    "   <name>Second int</name>" \
    "   <description>desc 2</description>" \
    "   <color>cyan</color>" \
    "   <changedcolor>magenta</changedcolor>" \
    "   <editable>false</editable>" \
    "</element>" \
    "<element>" \
    "   <size>4</size>" \
    "   <name>Third int</name>" \
    "   <description>desc 3</description>" \
    "   <color>yellow</color>" \
    "   <changedcolor>green</changedcolor>" \
    "   <editable>true</editable>" \
    "</element>" \
    "</color_scheme>"
#endif

QBinaryDataColorScheme::QBinaryDataColorScheme()
    : source_(0)
{
    root_.size          = 0;
    root_.name          = "no name";
    root_.description   = "no descrition";
    root_.color         = Qt::white;
    root_.changedcolor  = Qt::gray;
    root_.editable      = true;

    /* TODO: fill test scheme */
    //QByteArray scheme;
    //scheme += TEST_SCHEME;
    //parseScheme(scheme);
}

void QBinaryDataColorScheme::setDataSource(QAbstractBinaryDataSource *source)
{
    source_ = source;
    if (source_)
    {
        source_->setColorScheme(this);
    }
}

void QBinaryDataColorScheme::parseChildElement(QDomElement *element)
{
    Element child = root_;
    child.editable = true;

    QDomNode n = element->firstChild();
    while (!n.isNull())
    {
        QDomElement e = n.toElement();
        if (!e.isNull())
        {
            QString tagName = e.tagName();
            if (tagName == "size")
            {
                child.size = e.text().toInt();
            }
            else if(tagName == "name")
            {
                child.name = e.text();
            }
            else if(tagName == "descrition")
            {
                child.description = e.text();
            }
            else if(tagName == "color")
            {
                if (!QColor::isValidColor(e.text()))
                {
                    qWarning() << "Unable to set color named as:" << e.text();
                }
                else
                {
                    child.color.setNamedColor(e.text());
                }
            }
            else if(tagName == "changedcolor")
            {
                if (!QColor::isValidColor(e.text()))
                {
                    qWarning() << "Unable to set color named as:" << e.text();
                }
                else
                {
                    child.changedcolor.setNamedColor(e.text());
                }
            }
            else if(tagName == "editable")
            {
                child.editable = QVariant::fromValue(e.text()).toBool();
            }
        }
        n = n.nextSibling();
    }

    childs_.append(child);
}

bool QBinaryDataColorScheme::parseScheme(const QByteArray &xmlDocument)
{
    bool res        = false;
    int errorLine   = -1;
    int errorColumn = -1;
    QString errorMsg;

    childs_.clear();

    QDomDocument doc;
    if (!doc.setContent(xmlDocument, false, &errorMsg, &errorLine, &errorColumn))
    {
        qDebug() << "Xml parsing error. Msg:"
                 << errorMsg
                 << "Line:" << errorLine
                 << "Column:" << errorColumn;
        return res;
    }

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    while (!n.isNull())
    {
        QDomElement e = n.toElement();
        if (!e.isNull())
        {
            QString tagName = e.tagName();
            if (tagName == "size")
            {
                root_.size = e.text().toInt();
            }
            else if(tagName == "name")
            {
                root_.name = e.text();
            }
            else if(tagName == "descrition")
            {
                root_.description = e.text();
            }
            else if(tagName == "color")
            {
                if (!QColor::isValidColor(e.text()))
                {
                    qWarning() << "Unable to set color named as:" << e.text();
                }
                else
                {
                    root_.color.setNamedColor(e.text());
                }
            }
            else if(tagName == "changedcolor")
            {
                if (!QColor::isValidColor(e.text()))
                {
                    qWarning() << "Unable to set color named as:" << e.text();
                }
                else
                {
                    root_.changedcolor.setNamedColor(e.text());
                }
            }
            else if(tagName == "editable")
            {
                root_.editable = QVariant::fromValue(e.text()).toBool();
            }
            else if(tagName == "element")
            {
                parseChildElement(&e);
            }
        }
        n = n.nextSibling();
    }

    res = true;

    return res;
}

quint64 QBinaryDataColorScheme::readFromDS(const QModelIndex &index, quint64 size) const
{
    quint64 res = 0;
    QByteArray data = source_->read(index, size);

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
        qint64 offset = source_->indexToOffset(index);
        qint64 packet_offset = 0;

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
            while (packet_start <= offset)
            {
                prev_packet_start = packet_start;
                QModelIndex dataPointer = source_->offsetToIndex(packet_start);
                if (!dataPointer.isValid())
                {
                    qCritical("QBinaryDataColorScheme::findElementByIndex: Can't read dynamic size");
                    break;
                }
                quint64 packet_size = readFromDS(dataPointer, qAbs(root_.size));
                packet_start += packet_size + qAbs(root_.size);
            }
            offset = offset - prev_packet_start;
            packet_offset = prev_packet_start;
        }

        // here offset should point to the first byte of needed packet
        qint64 elem_start = 0;

        foreach(Element elem, childs_)
        {
            quint64 elem_end;
            if (elem.size > 0)
            {
                elem_end = elem_start + elem.size;
            }
            else if (elem.size < 0)
            {
                if ( (offset >= elem_start) && (offset < (elem_start + qAbs(elem.size))) )
                {
                    // size field, return root
                    return root_;
                }
                QModelIndex dataPointer = source_->offsetToIndex(packet_offset + elem_start);
                if (!dataPointer.isValid())
                {
                    qCritical("QBinaryDataColorScheme::findElementByIndex: Can't read dynamic size");
                    break;
                }
                quint64 elem_size = readFromDS(dataPointer, qAbs(elem.size));
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
