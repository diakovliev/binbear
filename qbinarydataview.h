#ifndef QBINARYDATAVIEW_H
#define QBINARYDATAVIEW_H

#include <QAbstractScrollArea>
#include "qbinarydataviewviewport.h"

class QBinaryDataView : public QAbstractScrollArea
{
    Q_OBJECT
    Q_PROPERTY(bool addressBarVisible READ addressBarVisible WRITE setAddressBarVisible USER true)
    Q_PROPERTY(bool presentationBarVisible READ presentationBarVisible WRITE setPresentationBarVisible USER true)
    Q_PROPERTY(bool isEditorMode READ isEditorMode WRITE setEditorMode USER true)

public:
    explicit QBinaryDataView(QWidget *parent = 0);
    virtual ~QBinaryDataView(void);

    void setDataSource(QAbstractBinaryDataSource *newDataSource);
    QAbstractBinaryDataSource *dataSource() const;

    // properties
    bool addressBarVisible() const;
    bool presentationBarVisible() const;
    bool isEditorMode() const;

    void resizeEvent(QResizeEvent * event);
    void paintEvent(QPaintEvent * event);
    void mouseMoveEvent( QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void focusInEvent(QFocusEvent * event);
    void focusOutEvent(QFocusEvent * event);
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);

signals:

public slots:
    void gotoAddress(quint64 pos);

    // properties
    void setAddressBarVisible(bool value);
    void setPresentationBarVisible(bool value);
    void setEditorMode(bool value);

private:
    QBinaryDataViewViewport *viewport_;

};

#endif // QBINARYDATAVIEW_H
