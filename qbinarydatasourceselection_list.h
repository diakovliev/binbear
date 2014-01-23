#ifndef QBINARYDATASOURCESELECTION_LIST_H
#define QBINARYDATASOURCESELECTION_LIST_H

#include "qabstractbinarydatasourceselection.h"

class QBinaryDataSourceSelection_List : public QAbstractBinaryDataSourceSelection
{
    Q_OBJECT
public:
    explicit QBinaryDataSourceSelection_List(QAbstractBinaryDataSource *dataSource);
    
    bool isSelected(const QModelIndex &index) const;

signals:
    
public slots:
    
};

#endif // QBINARYDATASOURCESELECTION_LIST_H
