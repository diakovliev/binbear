#ifndef QBINARYDATAVIEW_H
#define QBINARYDATAVIEW_H

#include <QAbstractScrollArea>
#include "qbinarydataviewviewport.h"

class QBinaryDataView : public QAbstractScrollArea
{
    Q_OBJECT
public:
    explicit QBinaryDataView(QWidget *parent = 0);
    virtual ~QBinaryDataView(void);

    void setDataSource(QBinaryDataSource *newDataSource);
    QBinaryDataSource *dataSource() const;

    void resizeEvent(QResizeEvent * event);
    void paintEvent(QPaintEvent * event);
    void mouseMoveEvent( QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void focusInEvent(QFocusEvent * event);
    void focusOutEvent(QFocusEvent * event);
    void keyPressEvent(QKeyEvent * e);

signals:

public slots:

private:
    QBinaryDataViewViewport *viewport_;

};

#endif // QBINARYDATAVIEW_H
