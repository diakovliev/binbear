#include "qbinarydatasourceselection_list.h"

QBinaryDataSourceSelection_List::QBinaryDataSourceSelection_List(QAbstractBinaryDataSource *dataSource) :
    QAbstractBinaryDataSourceSelection(dataSource)
{
}

bool QBinaryDataSourceSelection_List::isSelected(const QModelIndex &index) const
{
    return false;
}
