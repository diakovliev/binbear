#include <QDebug>

#include "qbinarydatacolorxmlschemeparser.h"
#include "qbinarydatacolorscheme.h"

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

QBinaryDataColorXmlSchemeParser::QBinaryDataColorXmlSchemeParser()
    : QBinaryDataColorAbstractSchemeParser()
{
}

QBinaryDataColorXmlSchemeParser::~QBinaryDataColorXmlSchemeParser()
{
}

void QBinaryDataColorXmlSchemeParser::parseChildElement(QBinaryDataColorScheme *scheme, QDomElement *element)
{
    QBinaryDataColorScheme::Element child = scheme->root_;
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

    scheme->childs_.append(child);
}

bool QBinaryDataColorXmlSchemeParser::parseScheme(QBinaryDataColorScheme *scheme, const QByteArray &xmlDocument)
{
    bool res        = false;
    int errorLine   = -1;
    int errorColumn = -1;
    QString errorMsg;

    scheme->childs_.clear();

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
                scheme->root_.size = e.text().toInt();
            }
            else if(tagName == "name")
            {
                scheme->root_.name = e.text();
            }
            else if(tagName == "descrition")
            {
                scheme->root_.description = e.text();
            }
            else if(tagName == "color")
            {
                if (!QColor::isValidColor(e.text()))
                {
                    qWarning() << "Unable to set color named as:" << e.text();
                }
                else
                {
                    scheme->root_.color.setNamedColor(e.text());
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
                    scheme->root_.changedcolor.setNamedColor(e.text());
                }
            }
            else if(tagName == "editable")
            {
                scheme->root_.editable = QVariant::fromValue(e.text()).toBool();
            }
            else if(tagName == "element")
            {
                parseChildElement(scheme, &e);
            }
        }
        n = n.nextSibling();
    }

    res = true;

    return res;
}

QBinaryDataColorScheme *QBinaryDataColorXmlSchemeParser::parseColorScheme(const QByteArray &input)
{
    QBinaryDataColorScheme *scheme = new QBinaryDataColorScheme();
    if (!parseScheme(scheme, input))
    {
        delete scheme;
        scheme = 0;
    }
    return scheme;
}
