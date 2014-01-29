#ifndef QBINARYDATASOURCEPROXY_COLORSCHEME_H
#define QBINARYDATASOURCEPROXY_COLORSCHEME_H

#include <QColor>
#include <QDomElement>
#include "qbinarydatasource.h"

class QBinaryDataSourceProxy_ColorScheme
{
protected:
    struct Element {
        quint64 size;           /* item size in bytes (can be 0 for the root element) */
        QString name;           /* item name */
        QString description;    /* item description */
        QColor  color;          /* regular background color */
        QColor  changedcolor;   /* changed item background color */
    };

public:
    QBinaryDataSourceProxy_ColorScheme(QBinaryDataSource *source);

    QString name(const QModelIndex &index) const;
    QString descrition(const QModelIndex &index) const;
    QColor color(const QModelIndex &index) const;
    QColor changedColor(const QModelIndex &index) const;

    bool parseScheme(const QByteArray &xmlDocument);

protected:
    Element findElementByIndex(const QModelIndex &index) const;
    void parseChildElement(const QDomElement &element);

private:
    QBinaryDataSource   *source_;

    Element             root_;
    QList<Element>      childs_;

};

#endif // QBINARYDATASOURCEPROXY_COLORSCHEME_H
