#ifndef QBINARYDATASOURCEPROXY_COLORSCHEME_H
#define QBINARYDATASOURCEPROXY_COLORSCHEME_H

#include <QColor>
#include <QModelIndex>

class QDomElement;
class QAbstractBinaryDataSource;

class QBinaryDataColorScheme
{
public:
    struct Element {
        qint64  size;           /* item size in bytes (can be 0 for the root element) */
        QString name;           /* item name */
        QString description;    /* item description */
        QColor  color;          /* regular background color */
        QColor  changedcolor;   /* changed item background color */
        bool    editable;       /* editable item or not */
    };

    QBinaryDataColorScheme();

    QString name(const QModelIndex &index = QModelIndex()) const;
    QString descrition(const QModelIndex &index = QModelIndex()) const;
    QColor color(const QModelIndex &index = QModelIndex()) const;
    QColor changedColor(const QModelIndex &index = QModelIndex()) const;
    bool editable(const QModelIndex &index) const;

    void setDataSource(QAbstractBinaryDataSource *source);

protected:
    Element findElementByIndex(const QModelIndex &index) const;
    quint64 readFromDS(quint64 offset, quint64 size) const;

private:
    QAbstractBinaryDataSource   *source_;

    Element                     root_;
    QList<Element>              childs_;

    friend class QBinaryDataColorXmlSchemeParser;
};

#endif // QBINARYDATASOURCEPROXY_COLORSCHEME_H
