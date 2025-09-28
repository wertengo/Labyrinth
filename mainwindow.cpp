#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QTabBar>
#include<QFileDialog>
#include <QTextEdit>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_labyrinth(new Labyrinth())
{
    ui->setupUi(this);
    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->tabBar()->setCurrentIndex(0);
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

