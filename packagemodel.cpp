#include "packagemodel.h"

PackageModel::PackageModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int PackageModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_model.size();
}

QVariant PackageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch(role) {
    case NameRole:
        return m_model.at(index.row()).packageName;
    case DescriptionRole:
        return m_model.at(index.row()).packageDesc;
    case CategoryRole:
        return m_model.at(index.row()).category;
    case IsInstalledRole:
        return m_model.at(index.row()).isInstalled;
    case IsCheckedRole:
        return m_model.at(index.row()).isChecked;
    default:
        qWarning() << tr("Package model can't retrieve data from invalid role");
    }

    return QVariant();
}

bool PackageModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // Only allow the checked status to be updated
        if (role==IsCheckedRole && index.row() < m_model.size() && value.typeId() == QMetaType::Bool) {
            m_model[index.row()].isChecked = value.toBool();
        }
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags PackageModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> PackageModel::roleNames() const
{
    return {{NameRole, "name"},
            {DescriptionRole, "description"},
            {CategoryRole, "category"},
            {IsInstalledRole, "isInstalled"},
            {IsCheckedRole, "isChecked"}
    };
}

bool PackageModel::loadModel(QSettings &settings, const QStringList &installedPackageList)
{
    if(settings.allKeys().isEmpty()) {
        return false;
    }

    beginResetModel();
    m_model.clear();

    // Iterate over the settings file and populate the model
    const QStringList groups = settings.childGroups();
    for (const QString &group : groups) {
        settings.beginGroup(group);
        const QStringList keys = settings.allKeys();
        for (const QString &key : keys) {
            PackageData packageData;
            packageData.packageName = key;
            packageData.packageDesc = settings.value(key).toString();
            packageData.category = group;
            packageData.isInstalled = installedPackageList.contains(key);
            packageData.isChecked = false;
            m_model.append(packageData);
        }
        settings.endGroup();
    }

    endResetModel();

    return true;
}

void PackageModel::refresh(const QStringList &installedPackageList)
{

    beginResetModel();
    for (int i = 0; i<m_model.size(); i++) {
        m_model[i].isInstalled = installedPackageList.contains(m_model.at(i).packageName);
        m_model[i].isChecked = false;
    }

    endResetModel();
}

const QStringList PackageModel::getCheckedPackages()
{
    QStringList packageList;
    for (const PackageData &package : m_model) {
        if (package.isChecked) {
            packageList.append(package.packageName);
        }
    }
    return packageList;
}
