#ifndef PACKAGEMODEL_H
#define PACKAGEMODEL_H

#include <QAbstractListModel>
#include <QSettings>

struct PackageData {
    QString packageName;
    QString packageDesc;
    QString category;
    bool isInstalled;
    bool isChecked;
};

class PackageModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit PackageModel(QObject *parent = nullptr);

    enum {
        NameRole = Qt::UserRole,
        DescriptionRole,
        CategoryRole,
        IsInstalledRole,
        IsCheckedRole
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    bool loadModel(QSettings &settings, const QStringList &installedPackageList);

    void refresh(const QStringList &installedPackageList);

    const QStringList getCheckedPackages();

private:
    QList<PackageData> m_model;
};

#endif // PACKAGEMODEL_H
