#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    /*
    QPushButton btn;
    btn.setText("<b>Bold</b> <font color=blue><i>style</i></font>");
    btn.show();

    QLabel label(btn.text());
    label.show();
    */
    return a.exec();
}
