/*
    This file is part of the KDE libraries
    Copyright (C) 1999 Matthias Ettrich (ettrich@kde.org)
    Copyright (C) 2007 Lubos Lunak (l.lunak@kde.org)
    Copyright 2014 Martin Gräßlin <mgraesslin@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "windowinfo.h"
#include "kwindowsystem.h"
#include "waylandintegration.h"

#include <config-kwindowsystem.h>

#include <QRect>
#include <QGuiApplication>

#include <KWayland/Client/plasmashell.h>
#include <KWayland/Client/surface.h>


WindowInfo::WindowInfo(WId window, NET::Properties properties, NET::Properties2 properties2)
    : KWindowInfoPrivate(window, properties, properties2),
      m_valid(false),
      m_properties(properties),
      m_properties2(properties2),
      m_surface(KWayland::Client::Surface::fromQtWinId(window)),
      m_plasmaShellSurface(KWayland::Client::PlasmaShellSurface::get(m_surface))
{
    m_valid = m_surface != nullptr && m_surface->isValid();
}

WindowInfo::~WindowInfo()
{
}

bool WindowInfo::valid(bool withdrawn_is_valid) const
{
    Q_UNUSED(withdrawn_is_valid)
    return m_valid;
}

NET::States WindowInfo::state() const
{
    return 0;
}

bool WindowInfo::isMinimized() const
{
    return false;
}

NET::MappingState WindowInfo::mappingState() const
{
    return NET::Visible;
}

NETExtendedStrut WindowInfo::extendedStrut() const
{
    return NETExtendedStrut();
}

NET::WindowType WindowInfo::windowType(NET::WindowTypes supported_types) const
{
    if (!m_plasmaShellSurface || !m_plasmaShellSurface->isValid()) {
        return NET::Unknown;
    }

    if (m_properties & NET::WMWindowType) {
        switch (m_plasmaShellSurface->role()) {
        case KWayland::Client::PlasmaShellSurface::Role::Normal:
            if (supported_types & NET::NormalMask) {
                return NET::Normal;
            }
            break;
        case KWayland::Client::PlasmaShellSurface::Role::Desktop:
            if (supported_types & NET::DesktopMask) {
                return NET::Desktop;
            }
            break;
        case KWayland::Client::PlasmaShellSurface::Role::Panel:
            if (supported_types & NET::DockMask) {
                return NET::Dock;
            }
            break;
        case KWayland::Client::PlasmaShellSurface::Role::OnScreenDisplay:
            if (supported_types & NET::OnScreenDisplayMask) {
                return NET::OnScreenDisplay;
            }
            break;
        default:
            break;
        }
    }

    return NET::Unknown;
}

QString WindowInfo::visibleName() const
{
    return QString();
}

QString WindowInfo::visibleNameWithState() const
{
    return QString();
}

QString WindowInfo::name() const
{
    return QString();
}

QString WindowInfo::visibleIconName() const
{
    return QString();
}

QString WindowInfo::visibleIconNameWithState() const
{
    return QString();
}

QString WindowInfo::iconName() const
{
    return QString();
}

bool WindowInfo::onAllDesktops() const
{
    return false;
}

bool WindowInfo::isOnDesktop(int desktop) const
{
    Q_UNUSED(desktop)
    return false;
}

int WindowInfo::desktop() const
{
    return 0;
}

QStringList WindowInfo::activities() const
{
    return QStringList();
}

QRect WindowInfo::geometry() const
{
    return QRect();
}

QRect WindowInfo::frameGeometry() const
{
    return QRect();
}

WId WindowInfo::transientFor() const
{
    return 0;
}

WId WindowInfo::groupLeader() const
{
    return 0;
}

QByteArray WindowInfo::windowClassClass() const
{
    return QByteArray();
}

QByteArray WindowInfo::windowClassName() const
{
    return QByteArray();
}

QByteArray WindowInfo::windowRole() const
{
    return QByteArray();
}

QByteArray WindowInfo::clientMachine() const
{
    return QByteArray();
}

bool WindowInfo::actionSupported(NET::Action action) const
{
    Q_UNUSED(action)
    return false;
}
