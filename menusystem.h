/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)GPL3+
 *
 * elokab - a lightweight, Qt based, desktop toolset
 * https://sourceforge.net/project/elokab/
 *
 * Copyright: 2013-2014 elokab team
 * Authors:
 *   Abou Zakaria <yahiaui@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H
//#include <QtDBus>
#include <QWidget>
#include <QMenu>
#include "power/powermain.h"
//#include "PowerUtils.h"
//#include "Tawhid_interface.h"

/**
 * @brief The MenuSystem class
 */
class MenuSystem : public QMenu
{
          Q_OBJECT
public:
          explicit MenuSystem();

signals:

public slots:
    /*!< PowerMain::hibernate */
          void hibernate();
            /*!< PowerMain::logout */
          void logout();
            /*!< PowerMain::reboot */
          void reboot();
            /*!< PowerMain::shutdown */
          void shutdown();
            /*!< PowerMain::suspend */
          void suspend();
            /*!< قفل الشاشة */
          void lockScreen();
          /*!< تحديث الايقونات عند تغير الاعدادات */
void refreshIcons();
private:
          PowerMain m_power;
          QAction *actShutdown;
          QAction *actReboot;
          QAction *actHibernate;
          QAction *actSuspend;
          QAction *m_actLokScreen;
          QAction * m_actLogout;
private slots:
          QString loadService(const QString &path);
   //         com::tawhid::session::TawhidInterface *Tawhid;
};

#endif // MENUSYSTEM_H
