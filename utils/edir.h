﻿#ifndef EDIR_H
#define EDIR_H

#include <QObject>
#include<QDir>
class Edir
{

public:
    explicit   Edir(){}
    //!/usr/bin/prog
    static QString progPath(const QString &prog);
    //!$XDG_DATA_DIRS  default /usr/share/elokab
    static QString dataDir();
    static QString dataAppDir();
    //!$XDG_DATA_HOME default $HOME/.local/share
    static QString dataHomeDir();//$HOME/.local/share
    //!$XDG_DATA_HOME default $HOME/.local/share/elokab
    static QString dataHomeAppDir();

    //!XDG_CONFIG_HOME default  $HOME/.config/elokab
    static QString configDir();
    //!XDG_CONFIG_HOME default  $HOME/.config
    static QString configHomeDir();

    //!default /usr/share/applications $HOME/.local/share/applications
    static QStringList applicationsDirs();
    //!$HOME/.local/share/applications
    static QString applicationsHomeDir();

     static QString libDir();

    //!$XDG_CACHE_HOME default $HOME/.cache/elokab/+appName
    static QString cachDir();
    //!
    static QString desktopDir();
    //!
    static QString documentsDir();
    //!
    static QString musicDir();
    //!
    static QString moviesDir();
    //!
    static QString picturesDir();
    //!
    static QString downloadDir();


     //! XDG_DESKTOP_DIR Name
    static QString desktopDirName();
    //! XDG_DOCUMENTS_DIR Name
    static QString documentsDirName();
    //! XDG_MUSIC_DIR Name
    static QString musicDirName();
    //! XDG_PICTURES_DIR Name
    static QString moviesDirName();
    //! XDG_VIDEOS_DIR Name
    static QString picturesDirName();
    //! XDG_DOWNLOAD_DIR Name
    static QString downloadDirName();

   static QString xdgDirName(const QString &name);
};

#endif // EDIR_H
