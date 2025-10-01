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
    void on_buttonFindPath_clicked();
    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;
    Labyrinth* m_labyrinth;
    void updateLabyrinthDisplay();
    void updatePathInTable();
    void updateTextTab();
    void updateTableTab();
    void setupTableWidget();
    void colorCell(int row, int col, int cellType);
};
#endif // MAINWINDOW_H
