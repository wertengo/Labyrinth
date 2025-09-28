#ifndef LABYRINTH_H
#define LABYRINTH_H
#include<QString>
#include<QFile>
#include<QTextStream>
#include<QDebug>
#include<QDir>

class Labyrinth
{
public:
    Labyrinth();
    void load(QString filleName);
    QString draw();
    void printMapDigital();
private:
    static const int SIZE = 10;
    int m_map[SIZE][SIZE];
};

#endif // LABYRINTH_H
