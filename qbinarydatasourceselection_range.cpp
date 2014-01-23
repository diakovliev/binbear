#include "qbinarydatasourceselection_range.h"
#include "qabstractbinarydatasource.h"

QBinaryDataSourceSelection_Range::QBinaryDataSourceSelection_Range(QAbstractBinaryDataSource *dataSource)
    : QAbstractBinaryDataSourceSelection(dataSource)
    , dataSource_(dataSource)
    , begin_()
    , end_()
{
}

bool QBinaryDataSourceSelection_Range::isSelected(const QModelIndex &index) const
{
    bool ret = false;

    Q_ASSERT(dataSource_ != 0);

    if (!index.isValid() || !begin_.isValid() || !end_.isValid())
    {
        return ret;
    }

    quint64 offsetOfBegin = dataSource_->indexToOffset(begin_);
    quint64 offsetOfEnd = dataSource_->indexToOffset(end_);
    quint64 offsetOfIndex = dataSource_->indexToOffset(end_);

    ret = (offsetOfIndex >= offsetOfBegin) && (offsetOfIndex <= offsetOfEnd);

    return ret;
}

QModelIndex QBinaryDataSourceSelection_Range::begin() const
{
    return begin_;
}

void QBinaryDataSourceSelection_Range::setBegin(const QModelIndex &begin)
{
    if (begin_ != begin)
        begin_ = begin;

    normalize();
}

QModelIndex QBinaryDataSourceSelection_Range::end() const
{
    return end_;
}

void QBinaryDataSourceSelection_Range::setEnd(const QModelIndex &end)
{
    if (end_ != end)
        end_ = end;

    normalize();
}

void QBinaryDataSourceSelection_Range::setRange(const QModelIndex &begin, const QModelIndex &end)
{
    if (begin_ != begin)
        begin_ = begin;

    if (end_ != end)
        end_ = end;

    normalize();
}

void QBinaryDataSourceSelection_Range::normalize()
{
    Q_ASSERT(dataSource_ != 0);

    if (begin_.isValid() && end_.isValid())
    {
        quint64 offsetOfBegin = dataSource_->indexToOffset(begin_);
        quint64 offsetOfEnd = dataSource_->indexToOffset(end_);
        if (offsetOfEnd < offsetOfBegin) {
            QModelIndex tmp = begin_;
            begin_ = end_;
            end_ = tmp;
        }
    }
}
