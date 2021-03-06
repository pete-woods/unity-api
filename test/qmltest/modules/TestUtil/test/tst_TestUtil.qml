/*
 * Copyright (C) 2012, 2013 Canonical, Ltd.
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
 */


import QtQuick 2.0
import QtTest 1.0
import TestUtil 0.1

TestCase {
    Rectangle {
        id: rect
    }

    MockObjectForInstanceOfTestChild {
        id: testObject
    }

    // Singletons need to be bound to a property and not-named-imported
    // for them to be able to be properly passed back to C++.
    // See https://bugreports.qt-project.org/browse/QTBUG-30730
    property var util: Util

    function test_direct() {
        compare(Util.isInstanceOf(rect, "QQuickRectangle"), true, "rect should be an instance of QQuickRectangle");
        compare(Util.isInstanceOf(util, "TestUtil"), true, "Util should be an instance of TestUtil");
        compare(Util.isInstanceOf(testObject, "MockObjectForInstanceOfTestChild"), true, "testObject should be an instance of MockObjectForInstanceOfTestChild");
    }

    function test_inherited() {
        compare(Util.isInstanceOf(rect, "QQuickItem"), true, "rect should be an instance of QQuickItem");
        compare(Util.isInstanceOf(rect, "QObject"), true, "rect should be an instance of QObject");
        compare(Util.isInstanceOf(util, "QObject"), true, "Util should be an instance of QObject");
        compare(Util.isInstanceOf(testObject, "MockObjectForInstanceOfTest"), true, "testObject should be an instance of MockObjectForInstanceOfTest");
        compare(Util.isInstanceOf(testObject, "QQuickRectangle"), true, "testObject should be an instance of QQuickRectangle");
    }

    function test_negative() {
        compare(Util.isInstanceOf(rect, "QQuickMouseArea"), false, "rect should not be an instance of MouseArea");
        compare(Util.isInstanceOf(util, "QQuickItem"), false, "Util should not be an instance of QQuickItem");
    }

    function test_undefined() {
        compare(Util.isInstanceOf(undefined, "QObject"), false, "passing undefined should fail");
    }
}
