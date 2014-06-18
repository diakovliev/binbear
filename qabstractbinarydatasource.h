#ifndef QABSTRACTBINARYDATASOURCE_H
#define QABSTRACTBINARYDATASOURCE_H

#include <QIODevice>
#include <QAbstractItemView>

#include "qbinarydatacolorscheme.h"

class QAbstractBinaryDataSource : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit QAbstractBinaryDataSource(QObject *parent = 0);
    virtual ~QAbstractBinaryDataSource();
    
    /* View management. This property is used to convert "sequence" to table. */
    virtual void setViewWidth(quint8 viewWidth);
    virtual quint8 viewWidth(void) const;

    /* Data navigation */
    virtual QModelIndex nextIndex(const QModelIndex &index) const = 0;
    virtual QModelIndex prevIndex(const QModelIndex &index) const = 0;

    virtual QModelIndex offsetToIndex(quint64 offset) const = 0;
    virtual qint64 indexToOffset(const QModelIndex &index) const = 0;

    virtual QByteArray read(const QModelIndex &from, quint64 size) const = 0;

    bool indexInRange(const QModelIndex &pos, const QModelIndex &p1, const QModelIndex &p2) const;

    void setColorScheme(QBinaryDataColorScheme *colorScheme);
    QBinaryDataColorScheme *colorScheme() const;

signals:
    
public slots:

private:
    /* View management */
    quint8 viewWidth_;
    QBinaryDataColorScheme *colorScheme_;

};

#endif // QABSTRACTBINARYDATASOURCE_H
