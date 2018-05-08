#include "powermain.h"
//#include "powerdlg.h"
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDebug>
#define HAL_SERVICE "org.freedesktop.Hal"
#define HAL_PATH "/org/freedesktop/Hal/devices/computer"
#define HAL_INTERFACE "org.freedesktop.Hal.Device.SystemPowerManagement"

#define UPOWER_SERVICE          "org.freedesktop.UPower"
#define UPOWER_PATH             "/org/freedesktop/UPower"
#define UPOWER_INTERFACE        UPOWER_SERVICE

#define SYSTEMD_SERVICE         "org.freedesktop.login1"
#define SYSTEMD_PATH            "/org/freedesktop/login1"
#define SYSTEMD_INTERFACE       "org.freedesktop.login1.Manager"

#define TAWHID_SERVICE      "org.tawhid.session"
#define TAWHID_PATH         "/Tawhid"
#define TAWHID_INTERFACE    "org.tawhid.session.TawhidInterface"



#define Management_SERVICE         "org.freedesktop.PowerManagement"
#define Management_PATH            "/org/freedesktop/PowerManagement"
#define Management_INTERFACE       Management_SERVICE

#define CONSOLEKIT_SERVICE      "org.freedesktop.ConsoleKit"
#define CONSOLEKIT_PATH         "/org/freedesktop/ConsoleKit/Manager"
#define CONSOLEKIT_INTERFACE    "org.freedesktop.ConsoleKit.Manager"

#define PROPERTIES_INTERFACE    "org.freedesktop.DBus.Properties"
PowerMain::PowerMain(QObject *parent) :
    QObject(parent)
{

}

//---------------------------------------
bool dbusCall(const QString &service,const QString &path, const QString &interface,const QString & method, bool sysd=false)
{
    QDBusInterface dbus(service, path, interface, QDBusConnection::systemBus());
    if (!dbus.isValid())
    {
        qWarning() << "dbusCall: QDBusInterface is invalid" << service <<  method;
        return false;
    }

    QDBusMessage msg ;
    if(sysd){
        msg = dbus.call(method);

       if (msg.arguments().isEmpty() || msg.arguments().first().isNull())
            return true;

        QString response = msg.arguments().first().toString();
        qDebug() << "systemd:" << method << "=" << response;
        return response == "yes" || response == "challenge";

    }else
        msg = dbus.call(method);

    return msg.arguments().isEmpty() ||
            msg.arguments().first().isNull() ||
            msg.arguments().first().toBool();
}

bool dbusExecCall(const QString & method,const QString & arg=QString())
{
    QDBusInterface dbus(SYSTEMD_SERVICE,SYSTEMD_PATH ,SYSTEMD_INTERFACE, QDBusConnection::systemBus());
    if (!dbus.isValid())
    {
        qWarning() << "dbusCall: QDBusInterface is invalid" << SYSTEMD_SERVICE <<  method;
        return false;
    }
    QDBusMessage msg ;
       if(arg.isEmpty())
        msg = dbus.call(method,true);
       else
          msg = dbus.call(method,"");

       if (msg.arguments().isEmpty() || msg.arguments().first().isNull())
            return true;

        QString response = msg.arguments().first().toString();
        qDebug() << "systemd:" << method << "=" << response;
        return response == "yes" || response == "challenge";


}
//-----------------------------------------------------------------------------------------
bool dbusGetProperty(const QString &service,const QString &path, const QString &interface,const QString & property)
{
    QDBusInterface dbus(service, path, interface,  QDBusConnection::systemBus());
    if (!dbus.isValid())
    {
         return false;
    }

    QDBusMessage msg = dbus.call("Get", dbus.interface(), property);

    return !msg.arguments().isEmpty() &&
            msg.arguments().first().value<QDBusVariant>().variant().toBool();
}
//--------------------------------------------------------------------------------------------
bool PowerMain::canAction(int action)
{
  //  qDebug()<<action;

    switch (action) {

    case Logout:
        return CanLogout();

    case Hibernate:
        return CanHibernate();

    case Reboot:
        return CanReboot();

    case Shutdown:
        return CanShutdown();

    case Suspend:
        return CanSuspend();

    default:
        return false;
    }


}
//--------------------------------------------------------------------------------------------
bool PowerMain::doAction(int action)
{

    if(action>4)return false;

//    if(message){
//        PowerDlg *dlg=new PowerDlg(action);
//        if(dlg->exec()!=QDialog::Accepted)
//            return false;
//    }
    switch (action) {

    case  Logout:
        return logout();

    case  Hibernate:
        return hibernate();

    case  Reboot:
        return reboot();

    case  Shutdown:
        return shutdown();

    case  Suspend:
        return suspend();

    default:
        return false;
    }

}
//--------------------------------------------------------------------------------------------
bool PowerMain::CanReboot()
{


    if(dbusCall(CONSOLEKIT_SERVICE,CONSOLEKIT_PATH ,CONSOLEKIT_INTERFACE , "CanRestart" ))
        return true;
    if( dbusCall(SYSTEMD_SERVICE,SYSTEMD_PATH ,SYSTEMD_INTERFACE ,  "CanReboot" ,true ))
        return true;
    return false;
}
//--------------------------------------------------------------------------------------------
bool PowerMain::CanShutdown()
{


    if(dbusCall(CONSOLEKIT_SERVICE,CONSOLEKIT_PATH ,CONSOLEKIT_INTERFACE ,   "CanStop" ))
        return true;
    if( dbusCall(SYSTEMD_SERVICE,SYSTEMD_PATH ,SYSTEMD_INTERFACE , "CanPowerOff" ,true ))
        return true;
    return false;
}
//--------------------------------------------------------------------------------------------
bool PowerMain::CanSuspend()
{


    if(dbusCall(Management_SERVICE,Management_PATH ,Management_INTERFACE ,  "CanSuspend"  ))
        return true;

    if( dbusCall(SYSTEMD_SERVICE,SYSTEMD_PATH ,SYSTEMD_INTERFACE , "CanSuspend" ,true ))
        return true;

    bool prop= dbusGetProperty(   UPOWER_SERVICE,UPOWER_PATH, PROPERTIES_INTERFACE, "CanSuspend");

    bool act=   dbusCall(UPOWER_SERVICE,UPOWER_PATH ,UPOWER_INTERFACE,"SuspendAllowed");


   // qDebug()<<prop<<act;

    return prop&&act;

}
//--------------------------------------------------------------------------------------------
bool PowerMain::CanHibernate()
{



    if(dbusCall(Management_SERVICE,Management_PATH ,Management_INTERFACE ,  "CanHibernate"  ))
        return true;

    if( dbusCall(SYSTEMD_SERVICE,SYSTEMD_PATH ,SYSTEMD_INTERFACE , "CanHibernate" ,true ))
        return true;
    bool prop= dbusGetProperty(   UPOWER_SERVICE,UPOWER_PATH, PROPERTIES_INTERFACE, "CanHibernate");

    bool act=   dbusCall(UPOWER_SERVICE,UPOWER_PATH ,UPOWER_INTERFACE,"HibernateAllowed");


   // qDebug()<<prop<<act;

    return prop&&act;
}
//--------------------------------------------------------------------------------------------
bool PowerMain::CanLogout()
{
//TODO fix this
//    QDBusInterface dbus(TAWHID_SERVICE,
//                        TAWHID_PATH,
//                        TAWHID_INTERFACE);

//    if (!dbus.isValid()) {
//        qDebug() << "elokab session is not valid!";
//        return false;
//    }
    return true;
}
//--------------------------------------------------------------------------------------------
bool PowerMain::reboot()
{


    if(dbusExecCall( "Reboot"))
        return true;

    if(dbusCall(CONSOLEKIT_SERVICE,CONSOLEKIT_PATH ,CONSOLEKIT_INTERFACE , "Restart" ))
          return true;

    return false;
}
//--------------------------------------------------------------------------------------------
bool PowerMain::shutdown()
{

    if(dbusExecCall( "PowerOff"))
        return true;

    if(dbusCall(CONSOLEKIT_SERVICE,CONSOLEKIT_PATH ,CONSOLEKIT_INTERFACE , "Stop" ))
        return true;
//    if( dbusCall(SYSTEMD_SERVICE,SYSTEMD_PATH ,SYSTEMD_INTERFACE, "PowerOff" ,true ))
//        return true;
    return false;
}
//--------------------------------------------------------------------------------------------
bool PowerMain::suspend()
{

    if(dbusExecCall( "Suspend"))
        return true;

    if(dbusCall(Management_SERVICE,Management_PATH ,Management_INTERFACE ,"Suspend"  ))
        return true;

//    if( dbusCall(SYSTEMD_SERVICE,SYSTEMD_PATH ,SYSTEMD_INTERFACE ,  "Suspend" ,true ))
//        return true;

    bool act=   dbusCall(UPOWER_SERVICE,UPOWER_PATH ,UPOWER_INTERFACE,"Suspend");

    return act;

}
//--------------------------------------------------------------------------------------------
bool PowerMain::hibernate()
{

    if(dbusExecCall( "Hibernate"))
        return true;

    if(dbusCall(Management_SERVICE,Management_PATH ,Management_INTERFACE , "Hibernate"  ))
        return true;
//    if( dbusCall(SYSTEMD_SERVICE,SYSTEMD_PATH ,SYSTEMD_INTERFACE ,  "Hibernate" ,true ))
//        return true;
    bool act=   dbusCall(UPOWER_SERVICE,UPOWER_PATH ,UPOWER_INTERFACE,"Hibernate");

    return act;
}
//--------------------------------------------------------------------------------------------
bool PowerMain::logout()
{
    QByteArray sS=qgetenv("DESKTOP_SESSION");
    qDebug()<<"envirenment"<<sS;
      if(dbusExecCall( "TerminateSession",sS))
          return true;

//          QDBusInterface dbus(SYSTEMD_SERVICE,SYSTEMD_PATH ,SYSTEMD_INTERFACE, QDBusConnection::systemBus());
//          if (!dbus.isValid())
//          {
//              qWarning() << "dbusCall: QDBusInterface is invalid" << SYSTEMD_SERVICE <<  "TerminateSession";
//              return false;
//          }
//          QDBusMessage msg ;

//              msg = dbus.call("TerminateSession","");

//             if (msg.arguments().isEmpty() || msg.arguments().first().isNull())
//                  return true;

//              QString response = msg.arguments().first().toString();
//              qDebug() << "systemd:" << "TerminateSession" << "=" << response;




    QDBusInterface dbus(TAWHID_SERVICE,
                        TAWHID_PATH,
                        TAWHID_INTERFACE);

    if (!dbus.isValid()) {
        qDebug() << "QDBusInterface is not valid!"<<TAWHID_SERVICE;
        return false;
    }
    dbus.call("logout");
    return false;
}
//--------------------------------------------------------------------------------------------
