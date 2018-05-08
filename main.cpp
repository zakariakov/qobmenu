#include "menuapplications.h"
#include "utils/edir.h"
#include <QApplication>
#include "panel_adaptor.h"
//#include <QAbstractNativeEventFilter>
#include <QDBusConnection>
#include <QSettings>
void helpMe()
{

    printf("Usage: elokab-terminal [OPTION]\n");
    puts("elokab-terminal v: 0.3 \n" );
    puts("OPTION:\n");
    puts(" -h  --help                      Print this help.\n");
    puts(" -s  --style      <int>          MenuStyle 0=classic 1=formMenu Default=0.\n");
    puts(" -p, --position   <int>          position of menu  0= cursorPos 1=topLeft 2=topRight 3=botLeft 4=botRight\n");
    puts(" -l, --layout     <int>          LayoutDirection 0=system 1=left-to-right 2= right-to-left\n");
    puts(" -b, --browser    <string>       costum file manager\n");
    puts(" -t, --terminal   <string>       costum terminal\n");
    puts(" -a, --at-point   <int,int>      space at pos (x,y) Default=0,0\n");
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDBusConnection connection = QDBusConnection::sessionBus();
    if (! QDBusConnection::sessionBus().registerService("org.elokab.qobmenu"))
    {
        qWarning() << "Unable to register 'org.elokab.qobmenu' service - is another instance of qobmenu running?";
        QDBusInterface dbus("org.elokab.qobmenu",
                            "/org/elokab/qobmenu",
                            "org.elokab.qobmenu.Interface");

        if (!dbus.isValid()) {
            qDebug() << "QDBusInterface is not valid!";
            return 0;
        }
        qDebug() << "QDBusInterface is valid!";

        dbus.call("showMenu");
        return 0;
    }


    a.setOrganizationName("elokab");
    a.setApplicationName("qobmenu");
    a.setApplicationDisplayName("qobmenu");

    //--------------------------Arguments------------------------------------
    QSettings s;


    int layout=s.value("LayoutDirection",0).toInt();



    QStringList args = a.arguments();
    if(args.count()>1)
    {

        for (int i = 0; i < args.count(); ++i) {
            QString arg = args.at(i);

            if (arg == "-h" || arg == "--help" ) {helpMe();return 0; }

            else if (arg == "-s" || arg == "--style" )  {
                int style=0;
                if(i+1>args.count()-1){helpMe();return 0;}
                style=QString(args.at(i+1)).toInt();
                if(style<0|| style>1)style=0;
                s.setValue("MenuStyle",style);

            }

            else if (arg == "-p" || arg == "--position" )  {
                int pos=0;
                if(i+1>args.count()-1){helpMe();return 0;}
                pos=QString(args.at(i+1)).toInt();
                if(pos<0|| pos>8)pos=0;
                s.setValue("MenuPos",pos);
            }

            else if (arg == "-l" || arg == "--layout" )  {
                if(i+1>args.count()-1){helpMe();return 0;}
                layout=QString(args.at(i+1)).toInt();
                if(layout<0 || layout>2)layout=0;
                s.setValue("LayoutDirection",layout);
            }

            else if (arg == "-b" || arg == "--browser" )  {
                QString browser=QString();
                if(i+1>args.count()-1){helpMe();return 0;}
                browser=QString(args.at(i+1));
                s.setValue("Browser",browser);
            }

            else if (arg == "-t" || arg == "--terminal" )  {
                QString terminal=QString();
                if(i+1>args.count()-1){helpMe();return 0;}
                terminal=QString(args.at(i+1));
                s.setValue("Terminal",terminal);
            }

            else if (arg == "-a" || arg == "--at-point" )  {
                if(i+1>args.count()-1){helpMe();return 0;}
                QString str=args.at(i+1);
                QStringList list=str.split(",");
                if(list.count()==2){
                    int x=QString(list.at(0)).toInt(0);
                    int y=QString(list.at(1)).toInt(0);

                    s.setValue("At-X",x);
                    s.setValue("At-Y",y);
                }
            }

        }

    }

    //---------------------------TRANSLATION------------------------------------
    /// مسار مجلد البرنامج
    QString translatPath= Edir::dataDir()+"/translations";
    /// جلب اعدادات اللغة
    QByteArray sS=qgetenv("DESKTOP_SESSION");
    QString locale;
    if(sS=="elokab-session"){
        QSettings globalSetting("elokab","elokabsettings");
        globalSetting.beginGroup("Language");
        locale=globalSetting.value("Name","Default").toString();
        globalSetting.endGroup();
        /// اذاكانت اللغة الافتراضية
        if(locale=="Default")
            locale = QLocale::system().name().section("_",0,0);
    }else{

        locale = QLocale::system().name().section("_",0,0);
    }

    /// اللغة الحالية لجميع البرنامج
    QLocale::setDefault(QLocale(locale));
    /// جلب ترجمات كيوتي
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += locale;
    QTranslator *translatorsys = new QTranslator;
    if (translatorsys->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        QApplication::installTranslator(translatorsys);
    /// جلب ترجمة البرنامج من مجلد البرنامج
    QTranslator translator;
    translator.load(QString(translatPath+"/"+locale+"/"+a.applicationName()));

    a.installTranslator(&translator);
    QLocale lx=QLocale(locale);
    switch (layout) {
    case 1:
        a.setLayoutDirection(Qt::LeftToRight);
        break;
    case 2:
        a.setLayoutDirection(Qt::RightToLeft);
        break;
    default:
        a.setLayoutDirection(lx.textDirection());
        break;
    }

    //-------------------------------------------------------------------------------end translation

    MenuApplications mp;
    new panel_adaptor(&mp);
    connection.registerObject("/org/elokab/qobmenu", &mp);
    mp.showMenu();

    return a.exec();
}
