#ifndef POWERMAIN_H
#define POWERMAIN_H
#include "powerdlg.h"
#include <QIcon>
#include <QObject>


/**
 * @brief The PowerMain class
 */
class PowerMain : public QObject
{
    Q_OBJECT
public:
    enum Action{
        Logout,    /// الخروج من الجلسة
        Hibernate, /// اسبات الجهاز
        Reboot,    /// اعادة اقلاع الجهاز
        Shutdown,  /// اغلاق الجهاز
        Suspend    /// اسبات الجهاز

    };
    static QIcon ICON ( Action act)

    {
      switch (act)
      {
      case Logout:
          return  QIcon::fromTheme("system-log-out",QIcon(":/icons/system-log-out"));
      case Hibernate:
          return QIcon::fromTheme("system-hibernate",QIcon(":/icons/system-hibernate"));
      case Reboot:
          return  QIcon::fromTheme("system-reboot",QIcon(":/icons/system-reboot"));
      case Shutdown:
          return QIcon::fromTheme("system-shutdown",QIcon(":/icons/system-shutdown"));
      case Suspend:
          return QIcon::fromTheme("system-suspend",QIcon(":/icons/system-suspend"));


      }
      return QIcon();
    }
    static QString TEXT ( Action act)

    {
      switch (act)
      {
      case Logout:
          return QObject::tr("Log out");
      case Hibernate:
          return QObject::tr("Hibernate");
      case Reboot:
          return  QObject::tr("Reboot");
      case Shutdown:
          return QObject::tr("Shutdown");
      case Suspend:
          return QObject::tr("Suspend");

      }
      return QString();
    }
    /**
     * @brief PowerMain
     * @param parent
     */
    explicit PowerMain(QObject *parent = 0);

signals:
    
public slots:
    /**
     * @brief canAction
     * @param action
     * @return
     */
    bool canAction(int action);
    /**
     * @brief doAction
     * @param action
     * @return
     */
    bool doAction(int action);



private slots:

    /*!< هل يمكن اعادة التشغيل */
    bool CanReboot();
    /*!< هل يمكن اطفاء الجهاز */
    bool CanShutdown();
    /*!< هل يمكن وضع الجهز في الاستعداد */
    bool CanSuspend();
    /*!< هل يمكن الاسبات */
    bool CanHibernate();
    /*!< هل يمكن الخروج من الجلسة */
    bool CanLogout();
    /*!< اعادة تضغيل الجهاز */
    bool reboot();
    /*!< اغلاق الجهاز */
    bool shutdown();
    /*!< استعداد الجهاز */
    bool suspend();
    /*!< اسبات الجهاز */
    bool hibernate();
    /*!< خروج من الجلسة */
    bool logout();


};

#endif // POWERMAIN_H
