#include "edir.h"
#include "desktopfile.h"

#include<QApplication>

#include<QDesktopServices>

#include<QTextCodec>
#include<QSettings>
#include<QDebug>
//_________________________________________________________________
QString Edir::progPath(const QString &prog)
{
    // "/usr/bin/prog"
    QDir dir(QApplication::applicationDirPath());

    if(QFile::exists(dir.absoluteFilePath(prog))){
        return dir.absoluteFilePath(prog);
    } else{
        QString path = qgetenv("PATH");
        foreach(QString dir, path.split(":"))
        {

            if (QFile::exists(dir + QDir::separator() + prog)){
                return dir + QDir::separator() + prog;
            }
        }
    }
    return prog;
}

//_________________________________________________________________
QString Edir::dataDir()
{
    //  "/usr/share/elokab
    QDir appDir(QApplication::applicationDirPath());
    QString dirPath;
    appDir.cdUp();
    dirPath=  appDir.absolutePath()+"/share/elokab";
    return dirPath;
}

//_________________________________________________________________
QString Edir::dataAppDir()
{
    //  "/usr/share/elokab/elokab-appname"
    QString appName=QApplication::applicationName();
    return dataDir()+QDir::separator()+appName;
}


//_________________________________________________________________
QString Edir::dataHomeDir()
{
    //"$Home/.local/share
    QDir dir(QDir::homePath()+"/.local/share");
    if(!dir.exists())
        dir.mkpath(".");

    return dir.absolutePath();
}

//_________________________________________________________________
QString Edir::dataHomeAppDir()
{
    //"$Home/.local/share/elokab/elokab-appname
    QString appName=QApplication::applicationName();
    QDir dir(dataHomeDir()+"/elokab/"+appName);
    if(!dir.exists())
        dir.mkpath(".");

    return dir.absolutePath();
}

//_________________________________________________________________
QString Edir::libDir()
{
    // "/usr/lib/elokab/elokabe-appname"
    QString appName=QApplication::applicationName();
    QDir pluginsDir(qApp->applicationDirPath());
    pluginsDir.cdUp();
    if(QFile::exists(pluginsDir.path()+"/lib/elokab/"+appName)){
        pluginsDir.cd(pluginsDir.path()+"/lib/elokab/"+appName);
    }else if(QFile::exists(pluginsDir.path()+"/lib64/elokab/"+appName)){
        pluginsDir.cd(pluginsDir.path()+"/lib64/elokab/"+appName);
    }
    return pluginsDir.absolutePath();
}

//_________________________________________________________________
QString Edir::configDir()
{
    //"$Home/.config/elokab"
    QDir dir(configHomeDir()+"/elokab");
    if(!dir.exists())
        dir.mkpath(".");
    return dir.absolutePath();
}

//_________________________________________________________________
QString Edir::configHomeDir()
{
    //"$Home/.config"
    QDir dir(QDir::homePath()+"/.config");
    if(!dir.exists())
        dir.mkpath(".");
    return dir.absolutePath();
}

//_________________________________________________________________
QStringList Edir::applicationsDirs()
{
    return QStringList()<<applicationsHomeDir()
                       <<"/usr/share/applications";
}

//_________________________________________________________________
QString Edir::applicationsHomeDir()
{
    QDir dir(QDir::homePath()+"/.local/share/applications");
    if(!dir.exists())
        dir.mkpath(".");
    return dir.absolutePath();
}

//_________________________________________________________________
QString Edir::cachDir()
{
    QString location;
#if QT_VERSION >= 0x050000
    location=QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#else
    location=QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
#endif

    QDir dir(location);
    if(!dir.exists())
        dir.mkpath(".");
    return location;
}

//_________________________________________________________________
QString Edir::desktopDir()
{
    QString location;
#if QT_VERSION >= 0x050000
    location=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
#else
    location=QDesktopServices::storageLocation(QDesktopServices::DesktopLocation);
#endif

    return location;
}

//_________________________________________________________________
QString Edir::documentsDir()
{
    QString location;
#if QT_VERSION >= 0x050000
    location=QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
#else
    location=QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
#endif

    return location;
}

//_________________________________________________________________
QString Edir::musicDir()
{
    QString location;
#if QT_VERSION >= 0x050000
    location=QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
#else
    location=QDesktopServices::storageLocation(QDesktopServices::MusicLocation);
#endif

    return location;
}

//_________________________________________________________________
QString Edir::moviesDir()
{
    QString location;
#if QT_VERSION >= 0x050000
    location=QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
#else
    location=QDesktopServices::storageLocation(QDesktopServices::MoviesLocation);
#endif

    return location;
}

//_________________________________________________________________
QString Edir::picturesDir()
{
    QString location;
#if QT_VERSION >= 0x050000
    location=QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
#else
    location=QDesktopServices::storageLocation(QDesktopServices::PicturesLocation);
#endif

    return location;
}

//_________________________________________________________________
QString Edir::downloadDir()
{
    QString location;
#if QT_VERSION >= 0x050000
    location=QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
#else
    location=QDesktopServices::storageLocation(QDesktopServices::DownloadLocation);
#endif

    return location;
}


// XDG_DESKTOP_DIR Name
QString Edir::desktopDirName()
{
    return xdgDirName("XDG_DESKTOP_DIR");
}

// XDG_DOCUMENTS_DIR Name
QString Edir::documentsDirName()
{
    return xdgDirName("XDG_DOCUMENTS_DIR");
}

//  XDG_MUSIC_DIR Name
QString Edir::musicDirName()
{
    return xdgDirName("XDG_MUSIC_DIR");
}

//  XDG_PICTURES_DIR Name
QString Edir::moviesDirName()
{
    return xdgDirName("XDG_VIDEOS_DIR");
}

// XDG_VIDEOS_DIR Name
QString Edir::picturesDirName()
{
    return xdgDirName("XDG_PICTURES_DIR");
}

//  XDG_DOWNLOAD_DIR Name
QString Edir::downloadDirName()
{
    return xdgDirName("XDG_DOWNLOAD_DIR");
}
//______________________
QString Edir::xdgDirName(const QString &name)
{
    QString fileName=(configHomeDir()+"/user-dirs.dirs");

    QFile files(fileName);
    if(!files.open( QFile::ReadOnly))
        return QString();

    QTextStream textStream(&files);
    textStream.setCodec(QTextCodec::codecForName("UTF-8"));
    QString line ;//premier line;

    while   (!textStream.atEnd()) {

        line = textStream.readLine().trimmed();
        if(line.startsWith("#"))
            continue;


        if (line.startsWith('[') && line.endsWith(']'))
            continue;

        if(!line.contains("="))
            continue;

        QString key=line.section("=",0,0).trimmed();
        QString value=line.section("=",1,1).trimmed();
        if(key!=name)
            continue;

        if (key.isEmpty())
            continue;
        value.remove("$HOME/");
        value.remove("\"");
        return value;
        //insert(key,value);



    }



    files.close();

    return QString();

}
