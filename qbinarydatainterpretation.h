#ifndef QBINARYDATAINTERPRETATION_H
#define QBINARYDATAINTERPRETATION_H

#include <QObject>
#include <QVariant>

/*
  The "object model" description:

  The QBinaryDataInterpretation works with the "data descriptions" which
 created using JSON based syntax. The data is interpreted as set of
 QBinaryDataRegion which have a set of predefined attributes.

  The QAbstactBinaryDataRegionType describes base interface to interpret binary
 data contained by area mentioned by QBinaryDataRegion.
 */

/* forward declarations */
class QBinaryDataSource;
class QBinaryDataRegion;

/*****************************************************************************/
class QAbstactBinaryDataRegionType {
public:
    /* region data reflection */
    virtual QVariant data(QBinaryDataRegion *region) const = 0;

};

/*****************************************************************************/
class QBinaryDataInterpretation : public QObject
{
Q_OBJECT
public:
    explicit QBinaryDataInterpretation(QObject *parent = 0);

    QBinaryDataRegion *region(quint64 pos);

signals:

public slots:

private:
    QBinaryDataSource *dataSource_;

};

/*****************************************************************************/
class QBinaryDataRegion : public QObject
{
Q_OBJECT
public:
    explicit QBinaryDataRegion(QObject *parent = 0);

    /* get region data */
    QVariant data() {
        Q_ASSERT(type_);
        return type_->data(this);
    }

signals:

public slots:

private:
    QAbstactBinaryDataRegionType *type_;
    QString name_;
    quint64 start_;
    quint64 size_;

};

#endif // QBINARYDATAINTERPRETATION_H
