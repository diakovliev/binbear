#ifndef QBINARYDATASOURCEPROXY_COLORSCHEME_H
#define QBINARYDATASOURCEPROXY_COLORSCHEME_H

#include <QColor>
#include <QModelIndex>

class QDomElement;
class QAbstractBinaryDataSource;

class QBinaryDataColorScheme
{
protected:
    struct Element {
        quint64 size;           /* item size in bytes (can be 0 for the root element) */
        QString name;           /* item name */
        QString description;    /* item description */
        QColor  color;          /* regular background color */
        QColor  changedcolor;   /* changed item background color */
        bool    editable;       /* editable item or not */
    };

public:
    QBinaryDataColorScheme();

    QString name(const QModelIndex &index = QModelIndex()) const;
    QString descrition(const QModelIndex &index = QModelIndex()) const;
    QColor color(const QModelIndex &index = QModelIndex()) const;
    QColor changedColor(const QModelIndex &index = QModelIndex()) const;
    bool editable(const QModelIndex &index) const;

    bool parseScheme(const QByteArray &xmlDocument);
    void setDataSource(QAbstractBinaryDataSource *source);

protected:
    Element findElementByIndex(const QModelIndex &index) const;
    void parseChildElement(QDomElement *element);

private:
    QAbstractBinaryDataSource   *source_;

    Element                     root_;
    QList<Element>              childs_;

};

#endif // QBINARYDATASOURCEPROXY_COLORSCHEME_H
