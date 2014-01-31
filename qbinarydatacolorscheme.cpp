#include <QDomDocument>
#include <QApplication>
#include <QDomElement>
#include <QDebug>

#include "qbinarydatacolorscheme.h"
#include "qabstractbinarydatasource.h"

#define TEST_SCHEME \
    "<color_scheme>" \
    "<size>188</size>" \
    "<name>PES</name>" \
    "<description>descrition</description>" \
    "<color>white</color>" \
    "<changedcolor>gray</changedcolor>" \
    "<element>" \
    "   <size>2</size>" \
    "   <name>First int</name>" \
    "   <description>desc 1</description>" \
    "   <color>red</color>" \
    "   <changedcolor>magenta</changedcolor>" \
    "</element>" \
    "<element>" \
    "   <size>2</size>" \
    "   <name>First int</name>" \
    "   <description>desc 1</description>" \
    "   <color>cyan</color>" \
    "   <changedcolor>magenta</changedcolor>" \
    "</element>" \
    "<element>" \
    "   <size>4</size>" \
    "   <name>Second int</name>" \
    "   <description>desc 1</description>" \
    "   <color>yellow</color>" \
    "   <changedcolor>green</changedcolor>" \
    "</element>" \
    "</color_scheme>"

QBinaryDataColorScheme::QBinaryDataColorScheme()
    : source_(0)
{
    root_.size          = 0;
    root_.name          = "no name";
    root_.description   = "no descrition";
    root_.color         = Qt::white;
    root_.changedcolor  = Qt::gray;

    /* TODO: fill test scheme */
//    QByteArray scheme;
//    scheme += TEST_SCHEME;
//    parseScheme(scheme);
}

void QBinaryDataColorScheme::setDataSource(QAbstractBinaryDataSource *source)
{
    source_ = source;
    source_->setColorScheme(this);
}

void QBinaryDataColorScheme::parseChildElement(QDomElement *element)
{
    Element child;

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

QBinaryDataColorScheme::Element
QBinaryDataColorScheme::findElementByIndex(const QModelIndex &index) const
{
    Element result = root_;
    if (source_ && index.isValid())
    {
        quint64 elem_start = 0;
        quint64 offset = source_->indexToOffset(index);

        if (root_.size)
        {
            offset = offset % root_.size;
        }
        foreach(Element elem, childs_)
        {
            quint64 elem_end = elem_start + elem.size;
            if (offset >= elem_start && offset < elem_end)
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
