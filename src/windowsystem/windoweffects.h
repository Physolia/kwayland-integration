/*
    SPDX-FileCopyrightText: 2014 Martin Gräßlin <mgraesslin@kde.org>
    SPDX-FileCopyrightText: 2015 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
#ifndef WINDOWEFFECTS_H
#define WINDOWEFFECTS_H
#include <kwindowsystem_version.h>
#include <private/kwindoweffects_p.h>

#include <QHash>
#include <QObject>
#include <QPointer>

namespace KWayland
{
namespace Client
{
class BlurManager;
class ContrastManager;
class Compositor;
class ConnectionThread;
class Blur;
class Contrast;
}
}

class WindowEffects : public QObject, public KWindowEffectsPrivateV2
{
    Q_OBJECT
public:
    WindowEffects();
    ~WindowEffects() override;

    static QWindow *windowForId(WId);

    bool eventFilter(QObject *watched, QEvent *event) override;
    void trackWindow(QWindow *window);
    void releaseWindow(QWindow *window);

    bool isEffectAvailable(KWindowEffects::Effect effect) override;
    void slideWindow(WId id, KWindowEffects::SlideFromLocation location, int offset) override;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QList<QSize> windowSizes(const QList<WId> &ids) override;

    void presentWindows(WId controller, const QList<WId> &ids) override;
    void presentWindows(WId controller, int desktop = NET::OnAllDesktops) override;
    void highlightWindows(WId controller, const QList<WId> &ids) override;
#endif
    void enableBlurBehind(WId winId, bool enable = true, const QRegion &region = QRegion()) override;
    void enableBackgroundContrast(WId winId,
                                  bool enable = true,
                                  qreal contrast = 1,
                                  qreal intensity = 1,
                                  qreal saturation = 1,
                                  const QRegion &region = QRegion()) override;
    void setBackgroundFrost(QWindow *window, QColor color, const QRegion &region = QRegion()) override;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    void markAsDashboard(WId window) override;
#endif
private:
    void installContrast(QWindow *window, bool enable = true, qreal contrast = 1, qreal intensity = 1, qreal saturation = 1, const QRegion &region = QRegion());
    void installBlur(QWindow *window, bool enable, const QRegion &region);
    void installSlide(QWindow *window, KWindowEffects::SlideFromLocation location, int offset);

    void resetBlur(QWindow *window, KWayland::Client::Blur *blur = nullptr);
    void resetContrast(QWindow *window, KWayland::Client::Contrast *contrast = nullptr);

    QHash<QWindow *, QList<QMetaObject::Connection>> m_windowWatchers;
    QHash<QWindow *, QRegion> m_blurRegions;
    struct BackgroundContrastData {
        qreal contrast = 1;
        qreal intensity = 1;
        qreal saturation = 1;
        QRegion region;
    };
    QHash<QWindow *, BackgroundContrastData> m_backgroundConstrastRegions;
    QHash<QWindow *, QPointer<KWayland::Client::Blur>> m_blurs;
    QHash<QWindow *, QPointer<KWayland::Client::Contrast>> m_contrasts;
    struct SlideData {
        KWindowEffects::SlideFromLocation location;
        int offset;
    };
    QHash<QWindow *, SlideData> m_slideMap;
};

#endif
