#ifndef QBINARYDATASOURCEPROXY_H
#define QBINARYDATASOURCEPROXY_H

#include "qabstractbinarydatasource.h"
#include "qbinarydatacolorscheme.h"

class QBinaryDataSource;

class QBinaryDataSourceProxy : public QAbstractBinaryDataSource
{
    Q_OBJECT
public:
    explicit QBinaryDataSourceProxy(QBinaryDataSource *source);
    virtual ~QBinaryDataSourceProxy();

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
    qint64 indexToOffset(const QModelIndex &index) const;

    void setViewWidth(quint8 viewWidth);
    quint8 viewWidth(void) const;

    QByteArray read(const QModelIndex &from, quint64 size) const;

    /* Do we have any changes */
    bool doWeHaveChangesToCommit();

    /* Commit/Revert changes */
    bool commitChanges();
    bool revertChanges();

signals:
    void dataChanged(const QModelIndex &index);

public slots:
    void onParentModelReset();

private:
    QBinaryDataSource *source_;
    QMap<QModelIndex, QVariant> cashedData_;
    
};

#endif // QBINARYDATASOURCEPROXY_H
