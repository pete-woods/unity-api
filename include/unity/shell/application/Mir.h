/*
 * Copyright (C) 2015-2017 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UNITY_SHELL_APPLICATION_MIR_H
#define UNITY_SHELL_APPLICATION_MIR_H

#include <QObject>

/**
    @brief Hold enums and global properties for use in QML
 */
class Mir : public QObject
{
    Q_OBJECT

    /**
        @brief Name of the mouse cursor to be used. Follows the X Cursor naming convention.

        Eg.: "left_ptr" is a left-sided pointer arrow
     */
    Q_PROPERTY(QString cursorName READ cursorName WRITE setCursorName NOTIFY cursorNameChanged)

    /**
      * Current (global) keymap, to be applied on the keyboard device
      *
      * E.g.: "cz+qwerty" -> "cz" layout, "qwerty" variant
      */
    Q_PROPERTY(QString currentKeymap READ currentKeymap WRITE setCurrentKeymap NOTIFY currentKeymapChanged)

    /**
      * @brief Whether mir pointer events should generate QTouchEvents instead of QMouseEvents
      *
      * If true, mir pointer events will be used to synthesize QTouchEvents instead of being translated
      * into QMouseEvents.
      *
      * This is useful when testing touch UIs with a mouse device.
      */
    Q_PROPERTY(bool touchEmulationEnabled READ touchEmulationEnabled WRITE setTouchEmulationEnabled NOTIFY touchEmulationEnabledChanged)

public:
    /**
        @brief Surface type
     */
    enum Type {
        UnknownType,
        NormalType,
        UtilityType,
        DialogType,
        GlossType,
        FreeStyleType,
        MenuType,
        InputMethodType,
        SatelliteType,
        TipType,
    };
    Q_ENUM(Type)

    /**
        @brief Surface state
     */
    enum State {
        UnknownState,
        RestoredState,
        MinimizedState,
        MaximizedState,
        FullscreenState,
        MaximizedLeftState,
        MaximizedRightState,
        HorizMaximizedState,
        VertMaximizedState,
        MaximizedTopLeftState,
        MaximizedTopRightState,
        MaximizedBottomLeftState,
        MaximizedBottomRightState,
        HiddenState,
    };
    Q_ENUM(State)

    /**
        @brief Surface orientation angle
     */
    enum OrientationAngle {
        Angle0 = 0,
        Angle90 = 90,
        Angle180 = 180,
        Angle270 = 270
    };
    Q_ENUM(OrientationAngle)

    /**
        @brief Shell chrome
     */
    enum ShellChrome {
        NormalChrome,
        LowChrome,
    };
    Q_ENUM(ShellChrome)

    /**
        @brief Form Factor
     */
    enum FormFactor {
        FormFactorUnknown,
        FormFactorPhone,
        FormFactorTablet,
        FormFactorMonitor,
        FormFactorTV,
        FormFactorProjector,
    };
    Q_ENUM(FormFactor)

    /// @cond
    virtual void setCursorName(const QString &cursorName) = 0;
    virtual QString cursorName() const = 0;

    virtual QString currentKeymap() const = 0;
    virtual void setCurrentKeymap(const QString &currentKeymap) = 0;

    virtual bool touchEmulationEnabled() const = 0;
    virtual void setTouchEmulationEnabled(bool enabled) = 0;
    /// @endcond

Q_SIGNALS:
    /// @cond
    void cursorNameChanged(const QString &cursorName);
    void currentKeymapChanged(const QString &currentKeymap);
    void touchEmulationEnabledChanged(bool enabled);
    /// @endcond
};

Q_DECLARE_METATYPE(Mir::Type)
Q_DECLARE_METATYPE(Mir::State)
Q_DECLARE_METATYPE(Mir::OrientationAngle)
Q_DECLARE_METATYPE(Mir::ShellChrome)
Q_DECLARE_METATYPE(Mir::FormFactor)

#endif // UNITY_SHELL_APPLICATION_MIR_H
