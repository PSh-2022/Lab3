#include "mainwindow.h"
#include <QApplication>
//файл для создания окна приложения и вывода в нем результатов работы программы
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
