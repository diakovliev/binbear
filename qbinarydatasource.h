#ifndef QBINARYDATASOURCE_H
#define QBINARYDATASOURCE_H

#include "qabstractbinarydatasource.h"

class QBinaryDataSourceProxy;

class QBinaryDataSource : public QAbstractBinaryDataSource
{
    Q_OBJECT
public:
    explicit QBinaryDataSource(QObject *parent = 0);
    virtual ~QBinaryDataSource(void);

    /* IO source */
    void attachTo(QIODevice *input);
    QIODevice *detachFrom(void);

    /* QAbstractItemModel */
    QModelIndex parent(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    /* Data navigation */
    QModelIndex nextIndex(const QModelIndex &index) const;
    QModelIndex prevIndex(const QModelIndex &index) const;

    QModelIndex offsetToIndex(quint64 offset) const;
    quint64 indexToOffset(QModelIndex index) const;

    /* Edit proxy */
    QBinaryDataSourceProxy *createProxy();

signals:

public slots:

private:
    /* IO source */
    QIODevice *ioDevice_;

};

#endif // QBINARYDATASOURCE_H
