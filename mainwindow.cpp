#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QTabBar>
#include<QFileDialog>
#include <QTextEdit>
#include <QStatusBar>
#include <QTextEdit>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QFont>
#include <QBrush>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_labyrinth(new Labyrinth())
{
    ui->setupUi(this);
    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->tabBar()->setCurrentIndex(0);

    setupTableWidget();
}

void MainWindow::setupTableWidget()
{
    qDebug() << "Настройка TableWidget...";

    if(!ui->labyrintMapField) {
        qCritical() << "labyrintMapField не найден!";
        return;
    }

    const int SIZE = 10;

    ui->labyrintMapField->setRowCount(SIZE);
    ui->labyrintMapField->setColumnCount(SIZE);

    ui->labyrintMapField->setShowGrid(true);
    ui->labyrintMapField->setSelectionMode(QAbstractItemView::NoSelection);
    ui->labyrintMapField->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for(int i = 0; i < SIZE; i++) {
        ui->labyrintMapField->setRowHeight(i, 40);
        ui->labyrintMapField->setColumnWidth(i, 40);
    }

    ui->labyrintMapField->verticalHeader()->setVisible(false);
    ui->labyrintMapField->horizontalHeader()->setVisible(false);

    ui->labyrintMapField->setStyleSheet(
        "QTableWidget { gridline-color: #666666; }"
        "QTableWidget::item { border: 1px solid #666666; }"
        );

    qDebug() << "TableWidget настроен успешно";
}

void MainWindow::updatePathInTable()
{
    if(!ui->labyrintMapField) return;

    qDebug() << "Обновление пути в таблице...";

    for(int row = 0; row < 10; row++) {
        for(int col = 0; col < 10; col++) {
            QTableWidgetItem *item = ui->labyrintMapField->item(row, col);
            if(item) {
                int cellValue = m_labyrinth->getCell(row, col);
                bool isInPath = m_labyrinth->isPointInPath(row, col);

                if(isInPath && cellValue == 0) {
                    item->setText("*");
                    item->setBackground(QBrush(QColor(173, 216, 230)));
                    item->setForeground(Qt::blue);
                    qDebug() << "Ячейка [" << row << "," << col << "] - часть пути";
                } else if(cellValue == 0) {
                    item->setText(" ");
                    item->setBackground(QBrush(Qt::white));
                    item->setForeground(Qt::black);
                }
            }
        }
    }

    qDebug() << "Обновление пути завершено";
}

void MainWindow::updateTableTab()
{
    qDebug() << "Обновление таблицы...";

    if(!ui->labyrintMapField) {
        qCritical() << "labyrintMapField не доступен!";
        return;
    }

    const int SIZE = 10;

    for(int row = 0; row < SIZE; row++) {
        for(int col = 0; col < SIZE; col++) {
            int cellValue = m_labyrinth->getCell(row, col);
            qDebug() << "Ячейка [" << row << "," << col << "] =" << cellValue;

            QTableWidgetItem *item = new QTableWidgetItem();

            switch(cellValue) {
            case 0:
                item->setText(" ");
                break;
            case 1:
                item->setText("X");
                break;
            case 2:
                item->setText("S");
                break;
            case 3:
                item->setText("#");
                break;
            default:
                item->setText("?");
            }

            item->setTextAlignment(Qt::AlignCenter);
            item->setFont(QFont("Arial", 14, QFont::Bold));

            colorCell(row, col, cellValue);

            ui->labyrintMapField->setItem(row, col, item);
        }
    }

    updatePathInTable();

    qDebug() << "Табличное отображение лабиринта обновлено";

    ui->labyrintMapField->update();
    ui->labyrintMapField->repaint();
}


void MainWindow::colorCell(int row, int col, int cellType)
{
    if(!ui->labyrintMapField) return;

    QTableWidgetItem *item = ui->labyrintMapField->item(row, col);
    if(!item) return;

    QBrush background;
    QColor textColor = Qt::black;

    switch(cellType) {
    case 0:
        background = QBrush(Qt::white);
        break;
    case 1:
        background = QBrush(QColor(100, 100, 100));
        textColor = Qt::white;
        break;
    case 2:
        background = QBrush(QColor(144, 238, 144));
        break;
    case 3:
        background = QBrush(QColor(255, 182, 193));
        break;
    default:
        background = QBrush(Qt::yellow);
    }

    item->setBackground(background);
    item->setForeground(textColor);
}



MainWindow::~MainWindow()
{
    delete m_labyrinth;
    delete ui;
}

void MainWindow::loadLabyrinth(const QString& fileName)
{
    m_labyrinth->load(fileName);
    m_labyrinth->printMapDigital();
    QString mazeText = m_labyrinth->draw();
    statusBar()->showMessage("Лабиринт загружен: " + fileName, 3000);

    qDebug().noquote() << "Отрисованный лабиринт:\n" << mazeText;
    updateTableTab();

}

void MainWindow::updateLabyrinthDisplay()
{
    updateTableTab();
    QString mapText = m_labyrinth->draw();
    qDebug().noquote() << "Текущий лабиринт:\n" << mapText;
}

void MainWindow::updateTextTab()
{
    QString mazeText = m_labyrinth->draw();

    qDebug().noquote() << "Текстовая вкладка - лабиринт:\n" << mazeText;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index == 0) {
        updateTextTab();
    } else if(index == 1) {
        updateTableTab();
    }
}
void MainWindow::displayLabyrinthInfo(const QString& info)
{
    statusBar()->showMessage(info);

    qDebug() << info;
}

void MainWindow::on_buttonFileDialogOpen_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    QString::fromUtf8("Открыть файл"),
                                                    QDir::currentPath(),
                                                    "Texts (*.txt);;All files (*.*)");
    qDebug() << "File clicked: "<<fileName;

    if(!fileName.isEmpty()) {
        loadLabyrinth(fileName);
    }
}


void MainWindow::on_buttonFindPath_clicked()
{
    if(m_labyrinth->findPath()) {
        qInfo() << "Путь успешно найден!";
        updateLabyrinthDisplay();
        statusBar()->showMessage("Путь найден! Длина: " +
                                     QString::number(m_labyrinth->getPathLength()), 5000);

        QMessageBox::information(this, "Успех",
                                 "Путь найден!\nДлина пути: " +
                                     QString::number(m_labyrinth->getPathLength()));
    } else {
        qWarning() << "Не удалось найти путь!";
        statusBar()->showMessage("Путь не найден!", 3000);
        QMessageBox::warning(this, "Поиск пути",
                             "Не удалось найти путь от старта к выходу!");
    }
}

