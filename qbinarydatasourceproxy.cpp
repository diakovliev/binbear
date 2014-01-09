#include "qbinarydatasourceproxy.h"
#include "qbinarydatasource.h"

QBinaryDataSourceProxy::QBinaryDataSourceProxy(QBinaryDataSource *source)
    : QAbstractItemModel(source)
    , source_(source)
    , cashedData_()
{
}

QModelIndex QBinaryDataSourceProxy::parent(const QModelIndex &index) const
{
    Q_ASSERT(source_ != 0);
    return source_->parent(index);
}

QModelIndex QBinaryDataSourceProxy::index(int row, int column, const QModelIndex &parent) const
{
    Q_ASSERT(source_ != 0);
    return source_->index(row, column, parent);
}

int QBinaryDataSourceProxy::rowCount(const QModelIndex & parent) const
{
    Q_ASSERT(source_ != 0);
    return source_->rowCount(parent);
}

int QBinaryDataSourceProxy::columnCount(const QModelIndex & parent) const
{
    Q_ASSERT(source_ != 0);
    return source_->columnCount(parent);
}

Qt::ItemFlags QBinaryDataSourceProxy::flags(const QModelIndex &index) const
{
    Q_ASSERT(source_ != 0);
    return source_->flags(index);
}

QVariant QBinaryDataSourceProxy::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_ASSERT(source_ != 0);
    return source_->headerData(section, orientation, role);
}

QVariant QBinaryDataSourceProxy::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(source_ != 0);

    if ( (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::BackgroundRole) && cashedData_.contains(index))
    {
        if (role == Qt::BackgroundRole)
        {
            // Change background for changed values

        }
        else
        {
            return cashedData_.value(index);
        }
    }
    else
    {
        return source_->data(index, role);
    }
}

bool QBinaryDataSourceProxy::setData(const QModelIndex & index, const QVariant & value, int role)
{
    Q_ASSERT(source_ != 0);

    if (role != Qt::EditRole)
    {
        return source_->setData(index, value, role);
    }
    else
    {
        cashedData_.insert(index, value);
        return true;
    }
}
