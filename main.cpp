#include "mainwindow.h"
#include "labyrinth.h"

#include <QApplication>

using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    Labyrinth labyrinth;

    labyrinth.load("name.txt");
    labyrinth.printMapDigital();

    QString drawnMaze = labyrinth.draw();
    qDebug().noquote() << "Отрисованный лабиринт:\n" << drawnMaze;

    w.show();
    return a.exec();
}






