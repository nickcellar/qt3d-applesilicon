/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QT3DRENDER_RENDER_TEXTURE_H
#define QT3DRENDER_RENDER_TEXTURE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QOpenGLContext>
#include <QMutex>
#include <Qt3DRender/qtexture.h>
#include <Qt3DRender/qtexturedata.h>
#include <Qt3DCore/qbackendnode.h>
#include <Qt3DRender/private/handle_types_p.h>

QT_BEGIN_NAMESPACE

class QOpenGLTexture;

namespace Qt3DRender {

class QAbstractTextureProvider;

namespace Render {

class TextureManager;
class TextureImageManager;
class TextureDataManager;

typedef uint TextureDNA;

class Texture : public Qt3DCore::QBackendNode
{
public:
    Texture();
    ~Texture();
    void cleanup();

    void updateFromPeer(Qt3DCore::QNode *peer) Q_DECL_OVERRIDE;

    QOpenGLTexture* getOrCreateGLTexture() ;

    GLint textureId();

    bool isTextureReset() const;

    void sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e) Q_DECL_OVERRIDE;
    TextureDNA dna() const;

    void setTextureManager(TextureManager *manager);
    void setTextureImageManager(TextureImageManager *manager);
    void setTextureDataManager(TextureDataManager *manager);

    void updateAndLoadTextureImage();
    void addTextureImageData(HTextureImage handle);
    void removeTextureImageData(HTextureImage handle);

    void requestTextureDataUpdate();
    void addToPendingTextureJobs();
    void setTarget(QAbstractTextureProvider::Target target);
    void setSize(int width, int height, int depth);
    void setFormat(QAbstractTextureProvider::TextureFormat format);
    void setMipLevels(int mipmapLevels);

    inline QVector<HTextureImage> textureImages() const { return m_textureImages; }
    inline QAbstractTextureProvider::TextureFormat format() const { return m_format; }
    inline QAbstractTextureProvider::Target target() const { return m_target; }
    inline bool isAutoMipMapGenerationEnabled() const { return m_generateMipMaps; }

    inline QTextureDataFunctorPtr dataFunctor() const { return m_dataFunctor; }
    void setTextureDataHandle(HTextureData handle) { m_textureDataHandle = handle; }

private:
    QOpenGLTexture *m_gl;

    QOpenGLTexture *buildGLTexture();
    void setToGLTexture(QTexImageData *imgData);
    void setToGLTexture(TextureImage *rImg, QTexImageData *imgData);
    void updateWrapAndFilters();

    int m_width;
    int m_height;
    int m_depth;
    int m_layers;
    int m_mipLevels;
    bool m_generateMipMaps;
    QAbstractTextureProvider::Target m_target;
    QAbstractTextureProvider::TextureFormat m_format;
    QAbstractTextureProvider::Filter m_magnificationFilter;
    QAbstractTextureProvider::Filter m_minificationFilter;
    QTextureWrapMode::WrapMode m_wrapModeX;
    QTextureWrapMode::WrapMode m_wrapModeY;
    QTextureWrapMode::WrapMode m_wrapModeZ;
    float m_maximumAnisotropy;
    QAbstractTextureProvider::ComparisonFunction m_comparisonFunction;
    QAbstractTextureProvider::ComparisonMode m_comparisonMode;

    QTextureDataFunctorPtr m_dataFunctor;
    HTextureData m_textureDataHandle;

    QVector<HTextureImage> m_textureImages;

    bool m_isDirty;
    bool m_filtersAndWrapUpdated;
    bool m_dataUploadRequired;
    bool m_formatWasSpecified;
    bool m_unique;

    mutable QMutex m_lock;
    TextureDNA m_textureDNA;
    TextureManager *m_textureManager;
    TextureImageManager *m_textureImageManager;
    TextureDataManager *m_textureDataManager;

    void updateDNA();
};

class TextureFunctor : public Qt3DCore::QBackendNodeFunctor
{
public:
    explicit TextureFunctor(TextureManager *textureManager,
                                  TextureImageManager *textureImageManager,
                                  TextureDataManager *textureDataManager);

    Qt3DCore::QBackendNode *create(Qt3DCore::QNode *frontend) const Q_DECL_FINAL;
    Qt3DCore::QBackendNode *get(const Qt3DCore::QNodeId &id) const Q_DECL_FINAL;
    void destroy(const Qt3DCore::QNodeId &id) const Q_DECL_FINAL;

private:
    TextureManager *m_textureManager;
    TextureImageManager *m_textureImageManager;
    TextureDataManager *m_textureDataManager;
};

} // namespace Render
} // namespace Qt3DRender

QT_END_NAMESPACE

Q_DECLARE_METATYPE(Qt3DRender::Render::Texture*)

#endif // QT3DRENDER_RENDER_TEXTURE_H
