#include "labyrinth.h"

Labyrinth::Labyrinth(){

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            m_map[i][j] = 0;
        }
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

    if(row != SIZE) {
        qWarning() << "Внимание: файл содержит только" << row << "строк. Ожидается" << SIZE;
    }

    qInfo() << "Лабиринт успешно загружен из файла:" << file;
}

QString Labyrinth::draw(){

    QString result;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            switch (m_map[i][j]) {
            case 0:
                result += " ";
                break;
            case 1:
                result += "X";
                break;
            case 2:
                result += "S";
                break;
            case 3:
                result += "#";
                break;
            default:
                result += "?";
                break;
            }
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
