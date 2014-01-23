#ifndef QABSTRACTBINARYDATASOURCE_H
#define QABSTRACTBINARYDATASOURCE_H

#include <QIODevice>
#include <QAbstractItemView>

//class QBinaryDataSourceSelection_List;
//class QBinaryDataSourceSelection_Range;

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

    inline bool indexInRange(const QModelIndex &pos, const QModelIndex &p1, const QModelIndex &p2) const
    {
        if (!pos.isValid() || !p1.isValid() || !p2.isValid())
            return false;

        quint64 po    = indexToOffset(pos);
        quint64 po1   = qMin(indexToOffset(p1), indexToOffset(p2));
        quint64 po2   = qMax(indexToOffset(p1), indexToOffset(p2));
        return po >= po1 && po <= po2;
    }

//    virtual QBinaryDataSourceSelection_List *createSelection(const QModelIndex &index) = 0;
//    virtual QBinaryDataSourceSelection_Range *createSelection(const QModelIndex &begin, const QModelIndex &end) = 0;

signals:
    
public slots:

private:
    /* View management */
    quint8 viewWidth_;
    
};

#endif // QABSTRACTBINARYDATASOURCE_H
