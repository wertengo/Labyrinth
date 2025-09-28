#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "labyrinth.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void loadLabyrinth(const QString& fileName);
    void displayLabyrinthInfo(const QString& info);

private slots:
    void on_buttonFileDialogOpen_clicked();

private:
    Ui::MainWindow *ui;
    Labyrinth* m_labyrinth;
};
#endif // MAINWINDOW_H
