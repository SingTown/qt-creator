/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company.  For licensing terms and
** conditions see http://www.qt.io/terms-conditions.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef TOOLTIP_H
#define TOOLTIP_H

#include "../utils_global.h"

#include <QSharedPointer>
#include <QObject>
#include <QTimer>
#include <QRect>
#include <QFont>

/*
 * In its current form QToolTip is not extensible. So this is an attempt to provide a more
 * flexible and customizable tooltip mechanism for Creator. Part of the code here is duplicated
 * from QToolTip. This includes a private Qt header and the non-exported class QTipLabel, which
 * here serves as a base tip class. Please notice that Qt relies on this particular class name in
 * order to correctly apply the native styles for tooltips. Therefore the QTipLabel name should
 * not be changed.
 */

QT_BEGIN_NAMESPACE
class QPoint;
class QWidget;
QT_END_NAMESPACE

namespace Utils {
namespace Internal { class QTipLabel; }

class TipContent;

class QTCREATOR_UTILS_EXPORT ToolTip : public QObject
{
    Q_OBJECT
protected:
    ToolTip();

public:
    ~ToolTip();

    bool eventFilter(QObject *o, QEvent *event);

    static ToolTip *instance();

    static void show(const QPoint &pos, const TipContent &content, QWidget *w = 0);
    static void show(const QPoint &pos, const TipContent &content, QWidget *w, const QRect &rect);
    static void move(const QPoint &pos, QWidget *w);
    static void hide();
    static bool isVisible();

protected slots:
    void hideTipImmediately();

protected:
    void showInternal(const QPoint &pos, const TipContent &content, QWidget *w, const QRect &rect);

private:
    bool acceptShow(const TipContent &content, const QPoint &pos, QWidget *w, const QRect &rect);
    bool validateContent(const TipContent &content);
    void setUp(const QPoint &pos, const TipContent &content, QWidget *w, const QRect &rect);
    bool tipChanged(const QPoint &pos, const TipContent &content, QWidget *w) const;
    void setTipRect(QWidget *w, const QRect &rect);
    void placeTip(const QPoint &pos, QWidget *w);
    void showTip();
    void hideTipWithDelay();

    Internal::QTipLabel *m_tip;
    QWidget *m_widget;
    QRect m_rect;
    QTimer m_showTimer;
    QTimer m_hideDelayTimer;
};

} // namespace Utils

#endif // TOOLTIP_H