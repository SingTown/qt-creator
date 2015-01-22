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

#ifndef QMLPROFILERTRACEVIEW_H
#define QMLPROFILERTRACEVIEW_H

#include "qmlprofilermodelmanager.h"
#include <QQuickView>
#include <QWidget>
#include <QTimer>

namespace Analyzer { class IAnalyzerTool; }

namespace QmlProfiler {

class QmlProfilerModelManager;
namespace Internal {

class QmlProfilerStateManager;
class QmlProfilerViewManager;

// centralized zoom control
class ZoomControl : public QObject {
    Q_OBJECT
public:
    static const qint64 MAX_ZOOM_FACTOR = 1 << 12;

    ZoomControl(const QmlProfilerTraceTime *traceTime, QObject *parent = 0);
    ~ZoomControl(){}

    Q_INVOKABLE void setRange(qint64 startTime, qint64 endTime);
    Q_INVOKABLE qint64 startTime() const { return m_startTime; }
    Q_INVOKABLE qint64 endTime() const { return m_endTime; }
    Q_INVOKABLE qint64 duration() const { return m_endTime - m_startTime; }

    Q_INVOKABLE qint64 windowStart() const { return m_windowStart; }
    Q_INVOKABLE qint64 windowEnd() const { return m_windowEnd; }
    Q_INVOKABLE qint64 windowLength() const { return m_windowEnd - m_windowStart; }
    void setWindowLocked(bool lock) { m_windowLocked = lock; }

signals:
    void rangeChanged();
    void windowChanged();

private slots:
    void rebuildWindow();
    void moveWindow();

private:
    qint64 m_startTime;
    qint64 m_endTime;
    qint64 m_windowStart;
    qint64 m_windowEnd;

    const QmlProfilerTraceTime *m_traceTime;
    QTimer m_timer;
    bool m_windowLocked;
};

class QmlProfilerTraceView : public QWidget
{
    Q_OBJECT

public:
    explicit QmlProfilerTraceView(QWidget *parent, Analyzer::IAnalyzerTool *profilerTool, QmlProfilerViewManager *container, QmlProfilerModelManager *modelManager, QmlProfilerStateManager *profilerState);
    ~QmlProfilerTraceView();

    void reset();

    bool hasValidSelection() const;
    qint64 selectionStart() const;
    qint64 selectionEnd() const;
    void showContextMenu(QPoint position);

public slots:
    void clear();
    void selectByTypeId(int typeId);
    void selectBySourceLocation(const QString &filename, int line, int column);

private slots:
    void updateCursorPosition();

    void updateRange();

    void profilerDataModelStateChanged();

protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

signals:
    void gotoSourceLocation(const QString &fileUrl, int lineNumber, int columNumber);
    void typeSelected(int typeId);
    void resized();

private:
    void enableToolbar(bool);

private:
    class QmlProfilerTraceViewPrivate;
    QmlProfilerTraceViewPrivate *d;
};

class QmlProfilerQuickView : public QQuickView {
public:
    QmlProfilerQuickView(QmlProfilerTraceView *parent) : parent(parent) {}
protected:
    QmlProfilerTraceView *parent;
    bool event(QEvent *ev);
};

} // namespace Internal
} // namespace QmlProfiler

#endif // QMLPROFILERTRACEVIEW_H
