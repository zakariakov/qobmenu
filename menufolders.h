#ifndef MENUFOLDERS_H
#define MENUFOLDERS_H

#include <QMenu>

class MenuFolders : public QMenu
{
    Q_OBJECT
public:
    explicit MenuFolders(QWidget *parent = 0);
    
signals:
public slots:
     void refreshIcons();

private slots:
    void chargeMenus();

    void execFolder();
private:
    QAction *actHomeFolder;
    QAction *actDesktopFolder;
    QAction *actMusicFolder;
    QAction *actPicturesFolder;
    QAction *actDocumentsFolder;
    QAction *actMoviesFolder;
};

#endif // MENUFOLDERS_H
