#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // incorporate Recent Files menu as actionRecent's submenu
    ui->actionRecent->setMenu(myRecentFiles.menu());

    // connect clear action to the slot clear()
    connect(ui->actionClear_Recent, SIGNAL(triggered()),
            &myRecentFiles, SLOT(clear()));

    // connect activated() signal to the label
    connect(&myRecentFiles, SIGNAL(activated(QString)),
            ui->lOpenedFile, SLOT(setText(QString)));

    // just for test reason, add a non-existing file
    myRecentFiles.add("z:/non-existing/dummy.txt");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog fileDialog(0, "Choose a file or directory");

    QString openName = fileDialog.getOpenFileName();

    // add chosen file/directory with icon
    if (!openName.isEmpty()) {
        QFileIconProvider iconProvider;
        myRecentFiles.add(openName, iconProvider.icon(QFileInfo(openName)));
    }
}


// this slot is automatically called when menuFile2 is about to show
void MainWindow::on_menuFile2_aboutToShow()
{
    // clear the menu
    ui->menuFile2->clear();

    // add some custom actions and separators
    ui->menuFile2->addAction("Open...");
    ui->menuFile2->addAction("Save...");
    ui->menuFile2->addSeparator();

    // populate the menu with the recent list
    myRecentFiles.populateMenu(ui->menuFile2);

    // add some custom actions and separators
    ui->menuFile2->addSeparator();
    ui->menuFile2->addAction("Exit");
}
