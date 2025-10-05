#include "labyrinth.h"

Labyrinth::Labyrinth(){

    m_cols = 0;
    m_rows = 0;
//     for (int i = 0; i < SIZE; i++) {
//         for (int j = 0; j < SIZE; j++) {
//             m_map[i][j] = 0;
//         }
//     }

//     m_pathLength = 0;
//     for(int i = 0; i < MAX_PATH_SIZE; i++) {
//         m_path[i] = QPoint(-1, -1);
//     }
// }
}

void Labyrinth::initializeMap(int rows, int cols){
    m_rows = rows;
    m_cols = cols;

    m_map.resize(m_rows);
    for (int i = 0; i < m_rows; i++) {
        m_map[i].resize(m_cols);
        for (int j = 0; j < m_cols; j++) {
            m_map[i][j] = 0;
        }
    }
}

void Labyrinth::load(QString file){
    m_lastError.clear();
    QDir currentDir;
    QString currentPath = currentDir.absolutePath();
    qDebug() << "Текущая дирректория: " << currentPath;

    QFile fileLoad(file);
    QString filePath = currentDir.absoluteFilePath(fileLoad.fileName());
    qDebug() << "Дирректория файла: " << filePath;

    if(!fileLoad.open(QIODevice::ReadWrite)){
        m_lastError = "Не удалось открыть файл: " + fileLoad.errorString();
        qCritical() << m_lastError;
        return;
    }

    qInfo() << "Read file...";
    QTextStream in(&fileLoad);
    // int row = 0;

    QVector<QString> lines;
    int maxCols = 0;
    int startCount = 0;
    int exitCount = 0;

    while(!in.atEnd()){
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) {
            continue;
        }

        lines.append(line);
        if (line.length() > maxCols) {
            maxCols = line.length();
        }
    }

    // while(!in.atEnd() && row < SIZE){
    //     QString line = in.readLine();

    //     if(line.isEmpty()) continue;

    //     for(int col = 0; col < SIZE && col < line.length(); col++) {
    //         QChar ch = line[col];
    //         int value = ch.digitValue();

    //         if(value >= 0 && value <= 3) {
    //             m_map[row][col] = value;
    //         } else {
    //             qWarning() << "Некорректный символ в позиции [" << row << "," << col << "]: '" << ch << "'";
    //             m_map[row][col] = -1;
    //         }
    //     }
    //     row++;
    // }
    fileLoad.close();

    if(lines.isEmpty()) {
        m_lastError = "Файл пустой!";
        qCritical() << m_lastError;
        return;
    }

    int rows = lines.size();
    int cols = maxCols;

    qDebug() << "Размер лабиринта:" << rows << "x" << cols;
    initializeMap(rows, cols);

    bool hasInvalidChars = false;
    QString invalidCharPositions;

    for(int row = 0; row < rows && row < lines.size(); row++) {
        QString line = lines[row];
        for(int col = 0; col < cols && col < line.length(); col++) {
            QChar ch = line[col];
            int value = ch.digitValue();

            if(value >= 0 && value <= 3) {
                m_map[row][col] = value;
            } else {
                hasInvalidChars = true;
                invalidCharPositions += QString("[%1,%2]='%3' ").arg(row).arg(col).arg(ch);
                qWarning() << "Некорректный символ в позиции [" << row << "," << col << "]: '" << ch << "'";
                m_map[row][col] = 0;
            }

            if(value == 2) {
                startCount++;
            } else if(value == 3) {
                exitCount++;
            }
        }
    }

    if(hasInvalidChars) {
        m_lastError = "Обнаружены некорректные символы: " + invalidCharPositions;
        qCritical() << m_lastError;
        return;
    }

    if(startCount == 0) {
        m_lastError = "Ошибка: не найден старт (символ '2')!";
        qCritical() << m_lastError;
        return;
    } else if(startCount > 1) {
        m_lastError = QString("Ошибка: найдено %1 стартов, должен быть только один!").arg(startCount);
        qCritical() << m_lastError;
        return;
    }

    if(exitCount == 0) {
        m_lastError = "Ошибка: не найден выход (символ '3')!";
        qCritical() << m_lastError;
        return;
    } else if(exitCount > 1) {
        m_lastError = QString("Ошибка: найдено %1 выходов, должен быть только один!").arg(exitCount);
        qCritical() << m_lastError;
        return;
    }

    m_path.clear();

    // m_pathLength = 0;

    // if(row != SIZE) {
    //     qWarning() << "Внимание: файл содержит только" << row << "строк. Ожидается" << SIZE;
    // }

    qInfo() << "Лабиринт успешно загружен из файла:" << file;
    qInfo() << "Размер:" << m_rows << "x" << m_cols;
}

void Labyrinth::forceLoadLabyrinth(QString file){
    m_lastError.clear();

    QDir currentDir;
    QString currentPath = currentDir.absolutePath();
    qDebug() << "Принудительная загрузка. Текущая директория: " << currentPath;

    QFile fileLoad(file);
    QString filePath = currentDir.absoluteFilePath(file);
    qDebug() << "Директория файла: " << filePath;

    if(!fileLoad.open(QIODevice::ReadOnly)){
        m_lastError = "Не удалось открыть файл: " + fileLoad.errorString();
        qCritical() << m_lastError;
        return;
    }

    qInfo() << "Принудительная загрузка файла...";
    QTextStream in(&fileLoad);

    QVector<QString> lines;
    int maxCols = 0;
    int invalidCharCount = 0;

    while(!in.atEnd()){
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) {
            continue;
        }

        lines.append(line);
        if (line.length() > maxCols) {
            maxCols = line.length();
        }
    }
    fileLoad.close();

    if(lines.isEmpty()) {
        m_lastError = "Файл пустой!";
        qCritical() << m_lastError;
        return;
    }

    int rows = lines.size();
    int cols = maxCols;

    qDebug() << "Размер лабиринта:" << rows << "x" << cols;

    initializeMap(rows, cols);

    for(int row = 0; row < rows; row++) {
        QString line = lines[row];
        for(int col = 0; col < cols; col++) {
            if(col >= line.length()) {
                m_map[row][col] = 0;
                continue;
            }

            QChar ch = line[col];
            int value = ch.digitValue();

            if(value >= 0 && value <= 3) {
                m_map[row][col] = value;
            } else {
                m_map[row][col] = 0;
                invalidCharCount++;
                qWarning() << "Заменен некорректный символ в [" << row << "," << col << "]: '" << ch << "'";
            }
        }
    }

    m_path.clear();

    qInfo() << "Лабиринт успешно загружен в принудительном режиме:" << file;
    qInfo() << "Размер:" << m_rows << "x" << m_cols;
    qInfo() << "Заменено некорректных символов:" << invalidCharCount;

    m_lastError.clear();
}

QString Labyrinth::draw()
{
    QString result;

    // char drawMap[SIZE][SIZE];
    QVector<QVector<char>> drawMap(m_rows, QVector<char>(m_cols, ' '));

    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
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

    for(int i = 0; i < m_path.size(); i++) {
        QPoint point = m_path[i];
        if(isValidPosition(point.y(), point.x()) &&
            m_map[point.y()][point.x()] != 2 && m_map[point.y()][point.x()] != 3) {
            drawMap[point.y()][point.x()] = '*';
        }
    }

    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            result += drawMap[i][j];
            if(j < m_cols - 1) result += " ";
        }
        if(i < m_rows - 1){
            result += "\n";
        }
    }

    return result;
}

void Labyrinth::printMapDigital()
{
    qDebug() << "Текущая карта лабиринта (числа):";
    for(int i = 0; i < m_rows; i++) {
        QString row;
        for(int j = 0; j < m_cols; j++) {
            row += QString::number(m_map[i][j]) + " ";
        }
        qDebug() << row;
    }
}

bool Labyrinth::findPath(){
    QPoint start(-1,-1), exit(-1,-1);
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {

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

    // bool visited[SIZE][SIZE];
    QVector<QVector<bool>> visited(m_rows, QVector<bool>(m_cols, false));
    // for(int i = 0; i < m_rows; i++) {
    //     for(int j = 0; j < m_cols; j++) {
    //         visited[i][j] = false;
    //     }
    // }

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
            // m_pathLength = current.path.size();
            m_path.clear();
            for(int i = 0; i < current.path.size(); i++) {
                m_path.append(current.path[i]);
            }
            // for(int i = 0; i < m_pathLength && i < MAX_PATH_SIZE; i++) {
            //     m_path[i] = current.path[i];
            // }
            qInfo() << "Кратчайший путь найден! Длина:" << m_path.size();
            return true;
        }

        for(int i = 0; i < 4; i++) {
            int newRow = currentPoint.y() + dRow[i];
            int newCol = currentPoint.x() + dCol[i];

            if(isValidPosition(newRow,newCol) &&
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
    for(int i = 0; i < m_path.size(); i++) {
        if(m_path[i].x() == col && m_path[i].y() == row) {
            return true;
        }
    }
    return false;
}

bool Labyrinth::isValidPosition(int row, int col) const
{
    return (row >= 0 && row < m_rows && col >= 0 && col < m_cols);
}

int Labyrinth::getCell(int row, int col) const
{
    if(isValidPosition(row, col)) {
        return m_map[row][col];
    }
    return -1;
}
