#include <QApplication>
#include <QStyle>
#include <QPalette>
#include <QDebug>

#include "qbinarydatasourceproxy.h"
#include "qbinarydatacolorscheme.h"
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

Qt::ItemFlags QBinaryDataSourceProxy::flags(const QModelIndex &index) const
{
    Q_ASSERT(source_ != 0);

    Qt::ItemFlags flags = source_->flags(index);
    if (colorScheme() && !colorScheme()->editable(index))
    {
        flags &= ~Qt::ItemIsEditable;
    }
    return flags;
}

QVariant QBinaryDataSourceProxy::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(source_ != 0);

    if (role == Qt::BackgroundRole)
    {
        if (colorScheme())
        {
            if (cashedData_.contains(index))
            {
                return colorScheme()->changedColor(index);
            }
            else
            {
                return colorScheme()->color(index);
            }
        }
        else
        {
            return QVariant();
        }
    }
    else if ((role == Qt::DisplayRole || role == Qt::EditRole) && cashedData_.contains(index))
    {
        return cashedData_.value(index);
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
        QVariant orig = source_->data(index, Qt::EditRole);
        if (orig != value)
        {
            cashedData_.insert(index, value);
        }
        else if (cashedData_.contains(index))
        {
            cashedData_.remove(index);
        }

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

qint64 QBinaryDataSourceProxy::indexToOffset(const QModelIndex &index) const
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

void QBinaryDataSourceProxy::onParentModelReset()
{
    Q_ASSERT(source_ != 0);

    reset();
}

QByteArray QBinaryDataSourceProxy::read(const QModelIndex &from, quint64 size) const
{
    QByteArray result;

    Q_ASSERT(source_ != 0);

    if (!from.isValid())
    {
        qWarning("QBinaryDataSourceProxy::read: not valid index");
        return result;
    }

    QModelIndex current = from;
    quint64 count = 0;
    do {
        quint8 byte = 0;
        QString value;

        if (!current.isValid())
        {
            break;
        }

        if (cashedData_.contains(current))
        {
            value = cashedData_.value(current).toString();
        }
        else
        {
            value = source_->data(current, Qt::DisplayRole).toString();
        }
        byte = value.toInt(0,16);

        result.append(byte);

        current = nextIndex(current);
        ++count;
    } while (count < size);

    return result;
}

