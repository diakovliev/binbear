#include "qbinarydatasourceproxy_colorscheme.h"

QBinaryDataSourceProxy_ColorScheme::QBinaryDataSourceProxy_ColorScheme(QBinaryDataSource *source)
    : source_(source)
{
    /* TODO: fill test scheme */

    /*

      <color_scheme>
        <size>4</size>
        <name>root element</name>
        <description>descrition</description>
        <color>white</color>
        <changedcolor>green</changedcolor>
        <element>
            <size>1</size>
            <name>test element 1</name>
            <description>test descrition 1</description>
            <color>red</color>
            <changedcolor>cyan</changedcolor>
        </element>
        <element>
            <size>1</size>
            <name>test element 2</name>
            <description>test descrition 2</description>
            <color>green</color>
            <changedcolor>black</changedcolor>
        </element>
      </color_scheme>

    */
    root_.size = 188;
    root_.name = "test";
    root_.description = "description";
    root_.color = QColor(Qt::white);
    root_.changedcolor = QColor(Qt::gray);

    Element child;
    child.size = 4;
    child.name = "test 1";
    child.description = "description 1";
    child.color = QColor(Qt::cyan);
    child.changedcolor = QColor(Qt::magenta);
    childs_.append(child);

    child.size = 4;
    child.name = "test 2";
    child.description = "description 2";
    child.color = QColor(Qt::yellow);
    child.changedcolor = QColor(Qt::green);
    childs_.append(child);
}

QBinaryDataSourceProxy_ColorScheme::Element QBinaryDataSourceProxy_ColorScheme::findElementByIndex(const QModelIndex &index) const
{
    Element result = root_;
    quint64 elem_start = 0;
    quint64 offset = source_->indexToOffset(index);

    if (root_.size)
    {
        offset = offset % root_.size;
    }
    foreach(Element elem, childs_)
    {
        quint64 elem_end = elem_start + elem.size;
        if (offset >= elem_start && offset < elem_end)
        {
            result = elem;
            break;
        }
        elem_start = elem_end;
    }

    return result;
}

QString QBinaryDataSourceProxy_ColorScheme::name(const QModelIndex &index) const
{
    Element element = findElementByIndex(index);
    return element.name;
}

QString QBinaryDataSourceProxy_ColorScheme::descrition(const QModelIndex &index) const
{
    Element element = findElementByIndex(index);
    return element.description;
}

QColor QBinaryDataSourceProxy_ColorScheme::color(const QModelIndex &index) const
{
    Element element = findElementByIndex(index);
    return element.color;
}

QColor QBinaryDataSourceProxy_ColorScheme::changedColor(const QModelIndex &index) const
{
    Element element = findElementByIndex(index);
    return element.changedcolor;
}
