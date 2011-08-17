#include "mainwindow.h"
#include "ui_mainwindow.h"

// include for TabListMenu
#include <QSint>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // create TabListMenu
    QSint::TabListMenu *tabMenu = new QSint::TabListMenu(this);

    // assing ui->tabWidget to the tabMenu
    tabMenu->setTabs(ui->tabWidget);

    // create corner button
    QToolButton *tbTabsDropList = new QToolButton(this);
    tbTabsDropList->setArrowType(Qt::DownArrow);
    tbTabsDropList->setPopupMode(QToolButton::InstantPopup);
    ui->tabWidget->setCornerWidget(tbTabsDropList);

    // assing tabMenu to the tbTabsDropList
    tbTabsDropList->setMenu(tabMenu);


    // just for demo: connect cbUseIcons checkbox to the tabMenu's setAllowIcons() slot
    connect(ui->cbUseIcons, SIGNAL(toggled(bool)), tabMenu, SLOT(setAllowIcons(bool)));
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
