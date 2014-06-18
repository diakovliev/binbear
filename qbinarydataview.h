#ifndef QBINARYDATAVIEW_H
#define QBINARYDATAVIEW_H

#include <QAbstractScrollArea>
#include <QSettings>

#include "qbinarydataviewviewport.h"

class QBinaryDataView : public QAbstractScrollArea
{
    Q_OBJECT
    Q_PROPERTY(bool addressBarVisible READ addressBarVisible WRITE setAddressBarVisible USER true)
    Q_PROPERTY(bool presentationBarVisible READ presentationBarVisible WRITE setPresentationBarVisible USER true)
    Q_PROPERTY(bool isEditorMode READ isEditorMode WRITE setEditorMode USER true)
    Q_PROPERTY(unsigned int groupSize READ groupSize WRITE setGroupSize USER true)

public:
    struct Properties
    {
        bool addressBarVisible;
        bool presentationBarVisible;
        bool isEditorMode;
        unsigned int groupSize;

        // TODO:
#if 0
        void load(QSettings &settings);
        void save(QSettings &settings) const;
#endif
    };

    explicit QBinaryDataView(QWidget *parent = 0);
    virtual ~QBinaryDataView(void);

    void setDataSource(QAbstractBinaryDataSource *newDataSource);
    QAbstractBinaryDataSource *dataSource() const;

    void setProperties(const Properties &properties);
    void getProperties(Properties &properties);

    // properties
    bool addressBarVisible() const;
    bool presentationBarVisible() const;
    bool isEditorMode() const;
    unsigned int groupSize() const;

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
    void setGroupSize(unsigned int newGroupSize);

private:
    QBinaryDataViewViewport *viewport_;

};

#endif // QBINARYDATAVIEW_H
