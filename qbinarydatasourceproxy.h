#ifndef QBINARYDATASOURCEPROXY_H
#define QBINARYDATASOURCEPROXY_H

#include <QAbstractItemModel>

class QBinaryDataSource;

class QBinaryDataSourceProxy : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit QBinaryDataSourceProxy(QBinaryDataSource *source);

    /* QAbstractItemModel */
    QModelIndex parent(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    
signals:
    
public slots:

private:
    QBinaryDataSource *source_;
    QMap<QModelIndex, QVariant> cashedData_;
    
};

#endif // QBINARYDATASOURCEPROXY_H
