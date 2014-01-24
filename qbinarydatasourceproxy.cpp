#include <QApplication>
#include <QStyle>
#include <QPalette>
#include <QDebug>

#include "qbinarydatasourceproxy.h"
#include "qbinarydatasource.h"

QBinaryDataSourceProxy::QBinaryDataSourceProxy(QBinaryDataSource *source)
    : QAbstractBinaryDataSource(source)
    , source_(source)
    , cashedData_()
{
}

QBinaryDataSourceProxy::~QBinaryDataSourceProxy()
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

void QBinaryDataSourceProxy::setViewWidth(quint8 viewWidth)
{
    Q_ASSERT(source_ != 0);
    source_->setViewWidth(viewWidth);
}

quint8 QBinaryDataSourceProxy::viewWidth(void) const
{
    Q_ASSERT(source_ != 0);
    return source_->viewWidth();
}

QVariant QBinaryDataSourceProxy::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(source_ != 0);

    if ( (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::BackgroundRole) && cashedData_.contains(index))
    {
        if (role == Qt::BackgroundRole)
        {
            // Change background for changed values
            QStyle *currentStyle = QApplication::style();
            QPalette currentPalette = currentStyle->standardPalette();

            QColor result = currentPalette.color(QPalette::Window);
            return result;

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

        emit dataChanged(index);

        return true;
    }
}

/* Data navigation */
QModelIndex QBinaryDataSourceProxy::nextIndex(const QModelIndex &index) const
{
    Q_ASSERT(source_ != 0);
    return source_->nextIndex(index);
}

QModelIndex QBinaryDataSourceProxy::prevIndex(const QModelIndex &index) const
{
    Q_ASSERT(source_ != 0);
    return source_->prevIndex(index);
}

QModelIndex QBinaryDataSourceProxy::offsetToIndex(quint64 offset) const
{
    Q_ASSERT(source_ != 0);
    return source_->offsetToIndex(offset);
}

quint64 QBinaryDataSourceProxy::indexToOffset(QModelIndex index) const
{
    Q_ASSERT(source_ != 0);
    return source_->indexToOffset(index);
}

/* Do we have any changes */
bool QBinaryDataSourceProxy::doWeHaveChangesToCommit()
{
    Q_ASSERT(source_ != 0);
    return !cashedData_.isEmpty();
}

/* Commit/Revert changes */
bool QBinaryDataSourceProxy::commitChanges()
{
    bool ret = true;

    Q_ASSERT(source_ != 0);

    QList<QModelIndex> indexes = cashedData_.keys();
    foreach (QModelIndex index, indexes)
    {
        ret = ret && source_->setData(index, cashedData_.value(index), Qt::EditRole);
        if (ret)
        {
            cashedData_.remove(index);
        }
        else
        {
            qDebug() << "QBinaryDataSourceProxy:: Error on setting data for index:" << index;
            break;
        }
    }

    reset();

    return ret;
}

bool QBinaryDataSourceProxy::revertChanges()
{
    Q_ASSERT(source_ != 0);

    cashedData_.clear();
    reset();

    return true;
}
