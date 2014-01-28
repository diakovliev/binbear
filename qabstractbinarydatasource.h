#ifndef QABSTRACTBINARYDATASOURCE_H
#define QABSTRACTBINARYDATASOURCE_H

#include <QIODevice>
#include <QAbstractItemView>

class QAbstractBinaryDataSource : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit QAbstractBinaryDataSource(QObject *parent = 0);
    virtual ~QAbstractBinaryDataSource();
    
    /* View management. This property is used to convert "sequence" to table. */
    virtual void setViewWidth(quint8 viewWidth);
    virtual quint8 viewWidth(void) const;

    /* Data navigation */
    virtual QModelIndex nextIndex(const QModelIndex &index) const = 0;
    virtual QModelIndex prevIndex(const QModelIndex &index) const = 0;

    virtual QModelIndex offsetToIndex(quint64 offset) const = 0;
    virtual quint64 indexToOffset(QModelIndex index) const = 0;

    bool indexInRange(const QModelIndex &pos, const QModelIndex &p1, const QModelIndex &p2) const;

signals:
    
public slots:

private:
    /* View management */
    quint8 viewWidth_;
    
};

#endif // QABSTRACTBINARYDATASOURCE_H
