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
    bool hasErrors() const { return !m_lastError.isEmpty(); }
    QString getLastError() const { return m_lastError; }
    void clearErrors() { m_lastError.clear(); }
    void load(QString filleName);
    void forceLoadLabyrinth(QString fillieName);
    QString draw();
    void printMapDigital();
    bool findPath();
    bool isPointInPath(int row, int col) const;
    int getCell(int row, int col) const;
    // int getPathLength() const { return m_pathLength; }
    int getPathLength() const { return m_path.size(); }
    int getRows() const { return m_rows; }
    int getCols() const { return m_cols; }
private:
    struct PathNode {
        QPoint point;
        int distance;
        QVector<QPoint> path;

        PathNode() : distance(0) {}
    };

    QString m_lastError;

    // static const int SIZE = 10;
    int m_rows;
    int m_cols;
    static const int MAX_PATH_SIZE = 100;
    // int m_map[SIZE][SIZE];
    // std::vector<std::vector<int>> m_map;
    QVector<QVector<int>> m_map;
    QVector<QPoint> m_path;
    int m_pathLength;
    // QPoint m_path[MAX_PATH_SIZE];
    // std::vector<QPoint> m_path;
    bool isValidPosition(int row, int col) const;
    void initializeMap(int rows, int cols);
};

#endif // LABYRINTH_H
