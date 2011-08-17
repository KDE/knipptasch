#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

protected slots:
    void executeAction();

    void on_cbTheme_activated(int);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
