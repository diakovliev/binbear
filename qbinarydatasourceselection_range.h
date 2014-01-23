#ifndef QBINARYDATASOURCESELECTION_RANGE_H
#define QBINARYDATASOURCESELECTION_RANGE_H

#include "qabstractbinarydatasourceselection.h"

class QBinaryDataSourceSelection_Range : public QAbstractBinaryDataSourceSelection
{
    Q_OBJECT    
public:
    explicit QBinaryDataSourceSelection_Range(QAbstractBinaryDataSource *dataSource);

    bool isSelected(const QModelIndex &index) const;

    QModelIndex begin() const;
    void setBegin(const QModelIndex &begin);
    QModelIndex end() const;
    void setEnd(const QModelIndex &end);

    void setRange(const QModelIndex &begin, const QModelIndex &end);

protected:
    void normalize();

private:
    QAbstractBinaryDataSource *dataSource_;

    QModelIndex begin_;
    QModelIndex end_;

};

#endif // QBINARYDATASOURCESELECTION_RANGE_H
