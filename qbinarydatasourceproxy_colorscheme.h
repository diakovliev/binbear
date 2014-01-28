#ifndef QBINARYDATASOURCEPROXY_COLORSCHEME_H
#define QBINARYDATASOURCEPROXY_COLORSCHEME_H

#include <QColor>
#include "qbinarydatasource.h"

class QBinaryDataSourceProxy_ColorScheme
{
public:
    struct Element {
        quint64 size;           /* item size in bytes (can be 0 for the root element) */
        QString name;           /* item name */
        QString description;    /* item description */
        QColor  color;          /* regular background color */
        QColor  changedcolor;   /* changed item background color */
    };

    QBinaryDataSourceProxy_ColorScheme(QBinaryDataSource *source);

    QString name(const QModelIndex &index) const;
    QString descrition(const QModelIndex &index) const;
    QColor color(const QModelIndex &index) const;
    QColor changedColor(const QModelIndex &index) const;

protected:
    Element findElementByIndex(const QModelIndex &index) const;

private:
    QBinaryDataSource   *source_;

    Element             root_;
    QList<Element>      childs_;

};

#endif // QBINARYDATASOURCEPROXY_COLORSCHEME_H
