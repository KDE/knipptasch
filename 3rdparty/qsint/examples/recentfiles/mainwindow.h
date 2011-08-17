#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

// include for RecentFiles
#include <QSint>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private Q_SLOTS:
    void on_actionOpen_triggered();
    void on_menuFile2_aboutToShow();

private:
    Ui::MainWindow *ui;

    // instantiate QSint::RecentFiles
    QSint::RecentFiles myRecentFiles;
};

#endif // MAINWINDOW_H
