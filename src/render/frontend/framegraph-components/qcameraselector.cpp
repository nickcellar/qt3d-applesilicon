/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qcameraselector.h"
#include "qcameraselector_p.h"
#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qscenepropertychange.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {

QCameraSelector::QCameraSelector(QCameraSelectorPrivate &dd, QNode *parent)
    : QFrameGraphItem(dd, parent)
{
}

QCameraSelector *QCameraSelector::doClone(QNode *clonedParent) const
{
    Q_D(const QCameraSelector);
    QCameraSelector *cameraSelector = new QCameraSelector(clonedParent);

    Q_FOREACH (const QFrameGraphItem *fgChild, d->m_fgChildren)
        cameraSelector->appendFrameGraphItem(qobject_cast<QFrameGraphItem *>(fgChild->clone(cameraSelector)));

    if (d->m_camera != Q_NULLPTR)
        cameraSelector->setCamera(qobject_cast<QEntity *>(d->m_camera->clone(cameraSelector)));

    return cameraSelector;
}

QCameraSelectorPrivate::QCameraSelectorPrivate(Qt3D::QCameraSelector *qq)
    : QFrameGraphItemPrivate(qq)
    , m_camera(Q_NULLPTR)
{}

QCameraSelector::QCameraSelector(Qt3D::QNode *parent)
    :   QFrameGraphItem(*new QCameraSelectorPrivate(this), parent)
{}

void QCameraSelector::setCamera(QEntity *camera)
{
    Q_D(QCameraSelector);
    if (d->m_camera != camera) {
        d->m_camera = camera;
        emit cameraChanged();
        if (camera->parent() == this)
            QNode::addChild(camera);
        QScenePropertyChangePtr propertyChange(new QScenePropertyChange(ComponentUpdated, this));
        propertyChange->setPropertyName(QByteArrayLiteral("camera"));
        propertyChange->setValue(QVariant::fromValue(d->m_camera->uuid()));
        notifyObservers(propertyChange);
    }
}

QEntity *QCameraSelector::camera() const
{
    Q_D(const QCameraSelector);
    return d->m_camera;
}

} // Qt3D

QT_END_NAMESPACE
