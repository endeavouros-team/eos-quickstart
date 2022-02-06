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

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    /*
     * Populates the model from @p settings.  Each group except general is treated
     * as a category, each key is a package and each value a description.  All
     * packages are set as unchecked initially.  To set isInstalled the package
     * names are compared to @p installedPackageList
     *
     * Returns true on success, false otherwise
     */
    bool loadModel(QSettings &settings, const QStringList &installedPackageList);

    /*
     * Refreshes the model by unchecking all items in the model and revaluating
     * each items installed status against @p installedPackageList
     */
    void refresh(const QStringList &installedPackageList);

    /*
     * Returns a QStringList of package names for package which is checked
     */
    const QStringList getCheckedPackages();

private:
    QList<PackageData> m_model;
};

#endif // PACKAGEMODEL_H
