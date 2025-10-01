#ifndef LABYRINTH_H
#define LABYRINTH_H
#include<QString>
#include<QFile>
#include<QTextStream>
#include<QDebug>
#include<QDir>
#include<QPoint>
#include<QVector>
#include<QQueue>

class Labyrinth
{
public:
    Labyrinth();
    void load(QString filleName);
    QString draw();
    void printMapDigital();
    bool findPath();
    bool isPointInPath(int row, int col) const;
    int getCell(int row, int col) const;
    int getPathLength() const { return m_pathLength; }
private:
    struct PathNode {
        QPoint point;
        int distance;
        QVector<QPoint> path;
    };

    static const int SIZE = 10;
    static const int MAX_PATH_SIZE = 100;
    int m_map[SIZE][SIZE];
    int m_pathLength;
    QPoint m_path[MAX_PATH_SIZE];
    bool isValidPosition(int row, int col) const;
};

#endif // LABYRINTH_H
