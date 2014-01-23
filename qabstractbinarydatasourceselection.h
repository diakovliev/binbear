#ifndef QABSTRACTBINARYDATASOURCESELECTION_H
#define QABSTRACTBINARYDATASOURCESELECTION_H

#include <QModelIndex>
#include <QObject>

class QAbstractBinaryDataSource;

class QAbstractBinaryDataSourceSelection : public QObject
{
    Q_OBJECT
public:
    explicit QAbstractBinaryDataSourceSelection(QAbstractBinaryDataSource *dataSource);

    virtual bool isSelected(const QModelIndex &index) const = 0;

signals:
    
public slots:
    
};

#endif // QABSTRACTBINARYDATASOURCESELECTION_H
