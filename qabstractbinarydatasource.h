#ifndef QABSTRACTBINARYDATASOURCE_H
#define QABSTRACTBINARYDATASOURCE_H

#include <QIODevice>
#include <QAbstractItemView>

class QAbstractBinaryDataSource : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit QAbstractBinaryDataSource(QObject *parent = 0);
    
    /* Data navigation */
    virtual QModelIndex nextIndex(const QModelIndex &index) const = 0;
    virtual QModelIndex prevIndex(const QModelIndex &index) const = 0;

    virtual QModelIndex offsetToIndex(quint64 offset) const = 0;
    virtual quint64 indexToOffset(QModelIndex index) const = 0;

signals:
    
public slots:
    
};

#endif // QABSTRACTBINARYDATASOURCE_H
