#ifndef QBINARYDATACOLORABSTRACTSCHEMEPARSER_H
#define QBINARYDATACOLORABSTRACTSCHEMEPARSER_H

#include <QByteArray>

class QBinaryDataColorScheme;

class QBinaryDataColorAbstractSchemeParser
{
public:
    QBinaryDataColorAbstractSchemeParser();
    virtual ~QBinaryDataColorAbstractSchemeParser();

    virtual QBinaryDataColorScheme *parseColorScheme(const QByteArray &input) = 0;


};

#endif // QBINARYDATACOLORABSTRACTSCHEMEPARSER_H
