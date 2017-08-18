#include "treedata.h"

TreeData::TreeData(QObject *parent) : QObject(parent), _id(0)
{
}

TreeData::TreeData(const TreeData &that)
    : QObject(that.parent())
{
    _text = that._text;
    _id = that._id;
}

TreeData::~TreeData()
{
}

QString TreeData::text()
{
    return _text;
}

void TreeData::setText(QString text)
{
    _text = text;
    emit textChanged();
}

int TreeData::id()
{
    return _id;
}

void TreeData::setId(int id)
{
    _id = id;
    emit idChanged();
}
