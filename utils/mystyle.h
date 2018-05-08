#ifndef MYSTYLE_H
#define MYSTYLE_H


#include <QObject>

class MyStyle
{

public:
    explicit   MyStyle(){}
    /*------------------------------------------------/
     * arg %1 =first color %2= next color %3=forcolor /
    /------------------------------------------------*/

    //    ـــــــــــــــــــــــــــــــــــــــــــــــــــــــــ
    static  QString left_left_powerline(){
        return  QString(
                    "qconicalgradient(cx:1, cy:0.506, angle:358.2, stop:0.43"
                    " %1, stop:0.431"
                    " %2, stop:0.579 "
                    " %2, stop:0.58 "
                    " %1);");
    }

    //    ـــــــــــــــــــــــــــــــــــــــــــــــــــــــــ
    static QString right_left_powerlinr(){
        return  QString(
                    "qconicalgradient(cx:1, cy:0.506, angle:358.2, stop:0.43"
                    " %2, stop:0.431"
                    " %1, stop:0.579 "
                    " %1, stop:0.58 "
                    " %2);");
    }

    //    ـــــــــــــــــــــــــــــــــــــــــــــــــــــــــ
    static QString  right_Reverse_powerline()
    {
        return QString(" qconicalgradient(cx:0, cy:0.533955, angle:180, stop:0.429787 "
                       " %2, stop:0.431 "
                       " %1, stop:0.579 "
                       " %1, stop:0.58 "
                       " %2); \n");
    }

    //    ـــــــــــــــــــــــــــــــــــــــــــــــــــــــــ
    static QString  left_Reverse_powerline()
    {
        return QString(" qconicalgradient(cx:0, cy:0.533955, angle:180, stop:0.429787 "
                       " %1, stop:0.431 "
                       " %2, stop:0.579 "
                       " %2, stop:0.58 "
                       " %1);");
    }

    //    ـــــــــــــــــــــــــــــــــــــــــــــــــــــــــ
    static QString  right_curvature()
    {
        return QString("qconicalgradient(cx:1, cy:1, angle:315, stop:0.5 "
                       " %1, stop:0.501 "
                       " %2); \n" );
    }

    //    ـــــــــــــــــــــــــــــــــــــــــــــــــــــــــ
    static QString  left_curvature()
    {
        return QString(" qconicalgradient(cx:0, cy:1, angle:225, stop:0.5 "
                       " %1, stop:0.501 "
                       " %2); \n" );
    }

    //    ـــــــــــــــــــــــــــــــــــــــــــــــــــــــــ
    static QString  right_curvature_Reverse()
    {
        return QString("qconicalgradient(cx:1, cy:1, angle:315, stop:0.5 "
                       " %2, stop:0.501 "
                       " %1); \n" );
    }

    //    ـــــــــــــــــــــــــــــــــــــــــــــــــــــــــ
    static QString  left_curvature_Reverse()
    {
        return QString(" qconicalgradient(cx:0, cy:1, angle:225, stop:0.5 "
                       " %2, stop:0.501 "
                       " %1); \n" );
    }

    //    ـــــــــــــــــــــــــــــــــــــــــــــــــــــــــ
    static QString  toolbuton_normal_color()
    {

        return QString(  "QToolButton{\n"

                         "background-color: transparent;\n"
                         "border-radius: 0px;\n"
                         "border-left:  7px solid transparent;\n"
                         "border-right:  7px solid transparent;\n"
                         "}\n"
                         );
    }

    //  ـــــــــــــــــــــــــــــــــــــــــــــــــــــــــ
    static QString  toolbuton_checked_color()
    {

        return QString(  "QToolButton:checked{\n"
                         "background-color: %2;\n"
                         "color: %3;\n"
                         );
    }


    //    ـــــــــــــــــــــــــــــــــــــــــــــــــــــــــ
    static QString statusStyle(int style)
    {
        switch (style) {
        case 1://  
            return   QString( "background-color: "+left_left_powerline());
            break;

        case 2:// 
            return   QString("background-color: "+right_Reverse_powerline());
            break;

        case 3:// ◢
            return QString("background-color: "+right_curvature());
            break;

        case 4:// ◣
            return QString(" background-color: "+left_curvature());
            break;

        case 5: // ▇
            return  QString(" background-color: %1 ;color:%2;");
            break;

        default:
            return QString();
            break;

        }

        return QString();
    }

    //TaskBar  ـــــــــــــــــــــــــــــــــــــــــــــــــــــــــ
    static QString taskbarStyle(int Style)
    {

        switch (Style) {
        case 1://  

            return   (QString(
                          toolbuton_normal_color()+
                          toolbuton_checked_color()+
                          "border-right-color:"+left_left_powerline()+
                          "border-left-color:"+right_left_powerlinr()+
                          "}")
                      );
            break;
        case 2:// 

            return   (QString(
                          toolbuton_normal_color()+
                          toolbuton_checked_color()+
                          " border-left-color: "+left_Reverse_powerline()+
                          " border-right-color: "+right_Reverse_powerline()+
                          "}"));
            break;
        case 3:// ◢

            return QString(
                        toolbuton_normal_color()+
                        toolbuton_checked_color()+
                        " border-left-color: "+right_curvature_Reverse()+
                        " border-right-color: "+right_curvature()+
                        "}"
                        );
            break;
        case 4://  ◣

            return QString(
                        toolbuton_normal_color()+
                        toolbuton_checked_color()+
                        "  border-left-color: "+left_curvature_Reverse()+
                        " border-right-color: "+left_curvature()+
                        "}"

                        );
            break;
        case 5: // ▇

            return QString("QToolButton{ background-color: %1 ; color %3; border:  0px solid; border-radius: 0px;} \n"
                           +toolbuton_checked_color()+" } "

                           );


            break;
        default:

            break;
        }
        return QString("QToolButton{ background-color:%1; \n "
                       " border:  0px solid #ffffff;  \n"
                       " border-radius: 0px; } \n"
                       " QToolButton:checked{ background-color: palette(highlight) ; \n"
                       " color: palette(highlightedtext); \n"
                       "border:  0px solid #ffffff;  \n"
                       "border-radius: 0px;} \n"
                       );



    }

    //MENU  ـــــــــــــــــــــــــــــــــــــــــــــــــــــــــ
    static  QString menuColor()
    {
        return  QString(

                    "QMenu {"
                    " border-radius: %4px;\n"
                    " background-color: %1; "
                    " border: 1px solid %3;"
                    " color: %2;\n"
                    " }"

                    " QMenu::item:selected {"
                    " background-color: %2;"
                    " color: %1;\n"
                    " }"
                    ) ;
    }

};



#endif // MYSTYLE_H
