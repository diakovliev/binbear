#include "qabstractbinarydatasourceselection.h"
#include "qabstractbinarydatasource.h"

QAbstractBinaryDataSourceSelection::QAbstractBinaryDataSourceSelection(QAbstractBinaryDataSource *dataSource)
    : QObject(dataSource)
{
}
