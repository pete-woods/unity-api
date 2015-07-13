/*
 * Copyright 2013 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *      Michael Zanetti <michael.zanetti@canonical.com>
 */

#ifndef UNITY_SHELL_APPLICATION_APPLICATIONMANAGERINTERFACE_H
#define UNITY_SHELL_APPLICATION_APPLICATIONMANAGERINTERFACE_H

#include <unity/SymbolExport.h>

#include <QtCore/QObject>
#include <QtCore/QAbstractListModel>
#include <QtQml/QJSValue>

namespace unity
{
namespace shell
{
namespace application
{

class ApplicationInfoInterface;

/**
 * @brief The Application manager
 *
 * This is the main class to interact with Applications
 */

class UNITY_API ApplicationManagerInterface: public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(Roles)

    /**
     * @brief The count of the applications known to the manager.
     *
     * This is the same as rowCount, added in order to keep compatibility with QML ListModels.
     */
    Q_PROPERTY(int count READ count NOTIFY countChanged)

    /**
     * @brief The currently focused application.
     *
     * Use focusApplication() and unfocusCurrentApplication() to modify this.
     */
    Q_PROPERTY(QString focusedApplicationId READ focusedApplicationId NOTIFY focusedApplicationIdChanged)

    /**
     * @brief Register a Javascript function as a callback for when an application is asking to create a new surface
     *
     * Registers a Javascript callback function which ApplicationManager will call when an application is asking
     * Mir to create a new surface. The function is passed a 'surface' object with properties width, height and
     * appId. The shell can implement this function to return different width & height to override the geometry
     * requested by the client.
     *
     * Example QML:
     *
     * function surfaceSizer(surface) {
     *     surface.width = 400;
     *     if (surface.appId && surface.appId == "dialer-app") {
     *         surface.height = 300;
     *     }
     *     return surface;
     * }
     *
     * Component.onCompleted {
     *     ApplicationManager.surfaceAboutToBeCreatedCallback = surfaceSizer;
     * }
     * Component.onDestruction {
     *     ApplicationManager.surfaceAboutToBeCreatedCallback = null;
     * }
     *
     * Warning: the function must live in the QML context thread!
     */
    Q_PROPERTY(QJSValue surfaceAboutToBeCreatedCallback READ surfaceAboutToBeCreatedCallback
               WRITE setSurfaceAboutToBeCreatedCallback NOTIFY surfaceAboutToBeCreatedCallbackChanged)

protected:
    /// @cond
    ApplicationManagerInterface(QObject* parent = 0): QAbstractListModel(parent)
    {
        m_roleNames.insert(RoleAppId, "appId");
        m_roleNames.insert(RoleName, "name");
        m_roleNames.insert(RoleComment, "comment");
        m_roleNames.insert(RoleIcon, "icon");
        m_roleNames.insert(RoleStage, "stage");
        m_roleNames.insert(RoleState, "state");
        m_roleNames.insert(RoleFocused, "focused");

        connect(this, SIGNAL(rowsInserted(QModelIndex, int, int)), SIGNAL(countChanged()));
        connect(this, SIGNAL(rowsRemoved(QModelIndex, int, int)), SIGNAL(countChanged()));
        connect(this, SIGNAL(modelReset()), SIGNAL(countChanged()));
        connect(this, SIGNAL(layoutChanged()), SIGNAL(countChanged()));
    }
    /// @endcond

public:
    /**
     * @brief The Roles supported by the model
     *
     * See ApplicationInfoInterface properties for details.
     */
    enum Roles {
        RoleAppId = Qt::UserRole,
        RoleName,
        RoleComment,
        RoleIcon,
        RoleStage,
        RoleState,
        RoleFocused,
    };

    /// @cond
    virtual ~ApplicationManagerInterface() {}

    virtual QHash<int, QByteArray> roleNames() const
    {
        return m_roleNames;
    }

    int count() const {
        return rowCount();
    }

    virtual QString focusedApplicationId() const = 0;

    virtual QJSValue surfaceAboutToBeCreatedCallback() const = 0;
    virtual void setSurfaceAboutToBeCreatedCallback(const QJSValue &callback) = 0;
    /// @endcond

    /**
     * @brief Get an ApplicationInfo item (using stack index).
     *
     * Note: QML requires the full namespace in the return value.
     *
     * @param index the index of the item to get
     * @returns The item, or null if not found.
     */
    Q_INVOKABLE virtual unity::shell::application::ApplicationInfoInterface *get(int index) const = 0;

    /**
     * @brief Get an ApplicationInfo item (using the appId).
     *
     * Note: QML requires the full namespace in the return value.
     *
     * @param appId the appId of the item to get
     * @returns The item, or null if not found.
     */
    Q_INVOKABLE virtual unity::shell::application::ApplicationInfoInterface *findApplication(const QString &appId) const = 0;

    /**
     * @brief Request to focus a given application
     *
     * This will request the shell to focus the given application.
     *
     * @param appId The appId of the app to be focused.
     * @returns True if the request will processed, false if it was discarded (i.e. the given appid could not be found)
     */
    Q_INVOKABLE virtual bool requestFocusApplication(const QString &appId) = 0;

    /**
     * @brief Focus the given application.
     *
     * This will immediately focus the given application. Usually you should not use this
     * but instead call requestFocusApplication() in order to allow the shell to prepare
     * for the upcoming animation or even block the focus request (e.g. focus stealing prevention)
     *
     * @param appId The application to be focused.
     * @returns True if appId found and application focused, else false.
     */
    Q_INVOKABLE virtual bool focusApplication(const QString &appId) = 0;

    /**
     * @brief Unfocus the currently focused application.
     */
    Q_INVOKABLE virtual void unfocusCurrentApplication() = 0;

    /**
     * @brief Start an application.
     *
     * @param appId The appId for the application to be spawned.
     * @param arguments Any arguments to be passed to the process.
     * @returns The created application item if start successful, else null.
     */
    Q_INVOKABLE virtual unity::shell::application::ApplicationInfoInterface *startApplication(const QString &appId, const QStringList &arguments) = 0;

    /**
      * @brief Stops an application.
      *
      * @param appId The application to be stopped.
      * @returns True if application stop successful, else false (i.e. false if application was not running).
      */
    Q_INVOKABLE virtual bool stopApplication(const QString &appId) = 0;

Q_SIGNALS:
    /// @cond
    void countChanged();
    void surfaceAboutToBeCreatedCallbackChanged();
    /// @endcond

    /**
     * @brief Will be emitted right before the focused application changes.
     *
     * This can be used to prepare for an upcoming focus change. For example starting
     * an animation.
     */
    void focusRequested(const QString &appId);

    /**
     * @brief Will be emitted whenever the focused application changes.
     */
    void focusedApplicationIdChanged();

    /**
     * @brief Will be emitted when an application was added to the model.
     *
     * @param appId The appId of the application that was added.
     */
    void applicationAdded(const QString &appId);

    /**
     * @brief Will be emitted when an application was removed from the model.
     *
     * @param appId The appId of the application that was removed.
     */
    void applicationRemoved(const QString &appId);

protected:
    /// @cond
    QHash<int, QByteArray> m_roleNames;
    /// @endcond
};

} // namespace application
} // namespace shell
} // namespace unity

#endif // UNITY_SHELL_APPLICATIONMANAGER_APPLICATIONINFO_H
