#include "labyrinth.h"

Labyrinth::Labyrinth(){

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            m_map[i][j] = 0;
        }
    }

    m_pathLength = 0;
    for(int i = 0; i < MAX_PATH_SIZE; i++) {
        m_path[i] = QPoint(-1, -1);
    }
}

void Labyrinth::load(QString file){
    QDir currentDir;
    QString currentPath = currentDir.absolutePath();
    qDebug() << "Текущая дирректория: " << currentPath;

    QFile fileLoad(file);
    QString filePath = currentDir.absoluteFilePath(fileLoad.fileName());
    qDebug() << "Дирректория файла: " << filePath;

    if(!fileLoad.open(QIODevice::ReadWrite)){
        qCritical() << "Could not open file!";
        qCritical() << fileLoad.errorString();
        return;
    }

    qInfo() << "Read file...";
    QTextStream in(&fileLoad);
    int row = 0;

    while(!in.atEnd() && row < SIZE){
        QString line = in.readLine();

        if(line.isEmpty()) continue;

        for(int col = 0; col < SIZE && col < line.length(); col++) {
            QChar ch = line[col];
            int value = ch.digitValue();

            if(value >= 0 && value <= 3) {
                m_map[row][col] = value;
            } else {
                qWarning() << "Некорректный символ в позиции [" << row << "," << col << "]: '" << ch << "'";
                m_map[row][col] = -1;
            }
        }
        row++;
    }
    fileLoad.close();

    m_pathLength = 0;

    if(row != SIZE) {
        qWarning() << "Внимание: файл содержит только" << row << "строк. Ожидается" << SIZE;
    }

    qInfo() << "Лабиринт успешно загружен из файла:" << file;
}

QString Labyrinth::draw()
{
    QString result;

    char drawMap[SIZE][SIZE];

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            switch (m_map[i][j]) {
            case 0:
                drawMap[i][j] = ' ';
                break;
            case 1:
                drawMap[i][j] = 'X';
                break;
            case 2:
                drawMap[i][j] = 'S';
                break;
            case 3:
                drawMap[i][j] = '#';
                break;
            default:
                drawMap[i][j] = '?';
                break;
            }
        }
    }

    for(int i = 0; i < m_pathLength; i++) {
        QPoint point = m_path[i];
        if(point.x() >= 0 && point.x() < SIZE && point.y() >= 0 && point.y() < SIZE &&
            m_map[point.y()][point.x()] != 2 && m_map[point.y()][point.x()] != 3) {
            drawMap[point.y()][point.x()] = '*';
        }
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            result += drawMap[i][j];
            if(j < SIZE - 1) result += " ";
        }
        result += "\n";
    }

    return result;
}

void Labyrinth::printMapDigital()
{
    qDebug() << "Текущая карта лабиринта (числа):";
    for(int i = 0; i < SIZE; i++) {
        QString row;
        for(int j = 0; j < SIZE; j++) {
            row += QString::number(m_map[i][j]) + " ";
        }
        qDebug() << row;
    }
}

bool Labyrinth::findPath(){
    QPoint start(-1,-1), exit(-1,-1);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {

            if (m_map[i][j] == 2) {
                start = QPoint(j,i);
            }

            if (m_map[i][j] == 3) {
                exit = QPoint(j,i);
            }
        }
    }

    if(start.x() == -1 || exit.x() == -1){
        qWarning()<<"Не найдена стартовая позиция или выход!";
        return false;
    }

    qDebug() << "Поиск пути от" << start << "до" << exit;

    bool visited[SIZE][SIZE];
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            visited[i][j] = false;
        }
    }

    QQueue<PathNode> queue;

    PathNode startNode;
    startNode.point = start;
    startNode.distance = 0;
    startNode.path.append(start);
    queue.enqueue(startNode);
    visited[start.y()][start.x()] = true;

    int dRow[] = {-1, 0, 1, 0};
    int dCol[] = {0, 1, 0, -1};

    while(!queue.isEmpty()) {
        PathNode current = queue.dequeue();
        QPoint currentPoint = current.point;

        if(currentPoint == exit) {
            m_pathLength = current.path.size();
            for(int i = 0; i < m_pathLength && i < MAX_PATH_SIZE; i++) {
                m_path[i] = current.path[i];
            }
            qInfo() << "Кратчайший путь найден! Длина:" << m_pathLength;
            return true;
        }

        for(int i = 0; i < 4; i++) {
            int newRow = currentPoint.y() + dRow[i];
            int newCol = currentPoint.x() + dCol[i];

            if(newRow >= 0 && newRow < SIZE && newCol >= 0 && newCol < SIZE &&
                !visited[newRow][newCol] && m_map[newRow][newCol] != 1) {

                visited[newRow][newCol] = true;

                PathNode newNode;
                newNode.point = QPoint(newCol, newRow);
                newNode.distance = current.distance + 1;
                newNode.path = current.path;
                newNode.path.append(newNode.point);

                queue.enqueue(newNode);
            }
        }
    }

    qWarning() << "Кратчайший путь не найден!";
    return false;
}

bool Labyrinth::isPointInPath(int row, int col) const
{
    for(int i = 0; i < m_pathLength; i++) {
        if(m_path[i].x() == col && m_path[i].y() == row) {
            return true;
        }
    }
    return false;
}

bool Labyrinth::isValidPosition(int row, int col) const
{
    return (row >= 0 && row < SIZE && col >= 0 && col < SIZE);
}

int Labyrinth::getCell(int row, int col) const
{
    if(isValidPosition(row, col)) {
        return m_map[row][col];
    }
    return -1;
}
