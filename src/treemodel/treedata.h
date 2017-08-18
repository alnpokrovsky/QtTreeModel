#ifndef TREEDATA_H
#define TREEDATA_H

#include <QObject>

class TreeData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)

public:
    TreeData(QObject *parent = nullptr);
    TreeData(const TreeData &that);
    ~TreeData();

    QString text();
    void setText(QString text);

    int id();
    void setId(int id);

signals:
    void textChanged();
    void idChanged();

private:
    QString _text;
    int _id;
};

Q_DECLARE_METATYPE(TreeData)

#endif // TREEDATA_H
