#include "treeitem.h"
#include "treemodel.h"

#include "treedata.h"

#include <QStringList>

TreeModel::TreeModel(const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    _roleNameMapping[TreeModelRoleName] = "title";
    //m_roleNameMapping[TreeModelRoleDescription] = "id";

    QList<QVariant> rootData;
    rootData << "Title"; // << "id";
    _rootItem = new TreeItem(rootData);
    setupModelData(data.split(QString("\n")), _rootItem);
}

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    _roleNameMapping[TreeModelRoleName] = "title";
    //m_roleNameMapping[TreeModelRoleDescription] = "id";

    QList<QVariant> rootData;
    rootData << "Title"; // << "id";
    _rootItem = new TreeItem(rootData);

    initTestData();
}

TreeModel::~TreeModel()
{
    delete _rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    } else {
        return _rootItem->columnCount();
    }
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != TreeModelRoleName) //&& role != TreeModelRoleDescription)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(role - Qt::UserRole - 1);
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return _rootItem->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid()) {
        parentItem = _rootItem;
    } else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    TreeItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == _rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid()) {
        parentItem = _rootItem;
    } else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

QHash<int, QByteArray> TreeModel::roleNames() const
{
    return _roleNameMapping;
}

QVariant TreeModel::newTreeData(int id, const QString &text)
{
    TreeData *t = new TreeData(this);
    t->setText(text);
    t->setId(id);
    QVariant v;
    v.setValue(t);
    return v;
}

void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
    QList<TreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QList<QVariant> columnData;
//            for (int column = 0; column < columnStrings.count(); ++column)
//                columnData << newCustomType(columnStrings[column], position);
            columnData << newTreeData(columnStrings[0].toInt(), columnStrings[1]);

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            parents.last()->appendChild(columnData);
        }

        ++number;
    }
}

void TreeModel::initTestData()
{
    QList<QVariant> columnData;
    columnData << newTreeData(1, "1. Курс вождения КВБМ СВ-86");
    _rootItem->appendChild(columnData);

    columnData.clear();
    columnData << newTreeData(2, "3.1. Подготовительные упражнения КВБМ");
    _rootItem->child(0)->appendChild(columnData);

    columnData.clear();
    columnData << newTreeData(3, "Вариант 1");
    _rootItem->child(0)->child(0)->appendChild(columnData);
    columnData.clear();
    columnData << newTreeData(4, "Вариант 2");
    _rootItem->child(0)->child(0)->appendChild(columnData);

    columnData.clear();
    columnData << newTreeData(5, "3.2. Учебные упражнения КВБМ");
    _rootItem->child(0)->appendChild(columnData);

    columnData.clear();
    columnData << newTreeData(6, "Вариант 1");
    _rootItem->child(0)->child(1)->appendChild(columnData);
    columnData.clear();
    columnData << newTreeData(7, "Вариант 2");
    _rootItem->child(0)->child(1)->appendChild(columnData);
}
