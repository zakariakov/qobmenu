#ifndef MENURECENT_H
#define MENURECENT_H

#include <QObject>
#include <QMenu>
class MenuRecent : public QMenu
{
     Q_OBJECT
public:
    MenuRecent(QWidget *parent);
signals:
 void actionAdded(QAction *);

public slots:
   void addRecentAct(QAction *act);
   void lanchApplication();
private :
   void loadService();
   void loadSettings();
  void saveSettings();
  void chargeDesktopApp(const QString &filePath);
  void swapAction(QAction *action);
};

#endif // MENURECENT_H
