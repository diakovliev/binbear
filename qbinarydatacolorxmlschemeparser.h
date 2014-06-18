#ifndef QBINARYDATACOLORXMLSCHEMEPARSER_H
#define QBINARYDATACOLORXMLSCHEMEPARSER_H

#include <QDomElement>

#include "qbinarydatacolorabstractschemeparser.h"

class QBinaryDataColorXmlSchemeParser : public QBinaryDataColorAbstractSchemeParser
{
public:
    QBinaryDataColorXmlSchemeParser();
    ~QBinaryDataColorXmlSchemeParser();

    QBinaryDataColorScheme *parseColorScheme(const QByteArray &input);

protected:
    void parseChildElement(QBinaryDataColorScheme *scheme, QDomElement *element);
    bool parseScheme(QBinaryDataColorScheme *scheme, const QByteArray &xmlDocument);

};

#endif // QBINARYDATACOLORXMLSCHEMEPARSER_H
