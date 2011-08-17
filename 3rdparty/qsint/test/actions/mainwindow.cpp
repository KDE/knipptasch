#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSint>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->toolButton->setDefaultAction(ui->action1);

    ui->actionLabel->setDefaultAction(ui->action1);

    ui->actionBox->header()->setText("The Header");

    ui->actionBox->header()->setDefaultAction(ui->actionRedo);

    QToolButton *tb1 = ui->actionBox->createItem("test1");
    tb1->setDefaultAction(ui->action1);

    QToolButton *tb2 = ui->actionBox->createItem("test2");
    tb2->setDefaultAction(ui->action2);

    ui->actionBox->createItem(ui->action1);
    ui->actionBox->createItem(ui->action2);

    ui->actionBox->createItems(QList<QAction*>() << ui->actionRedo << ui->actionUndo);

    ui->actionBox->createSpacer();


    QSint::ActionGroup *ag1 = ui->actionPanel->createGroup(QPixmap(":/res/exportpdf.png"), "Expandable Header", true);
    ag1->addAction(ui->action1);
    ag1->addAction(ui->action2);

    QSint::ActionGroup *ag2 = ui->actionPanel->createGroup(QPixmap(":/res/exportpdf.png"), "Static Header", false);
    ag2->addAction(ui->action1);
    ag2->addAction(ui->action2);

    QSint::ActionGroup *ag3 = ui->actionPanel->createGroup();
    ag3->addAction(ui->action1);
    ag3->addAction(ui->action2);

    ui->actionPanel->addStretch();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::executeAction()
{
    QMessageBox::about(0, "Action clicked", "Do something here :)");
}


void MainWindow::on_cbTheme_activated(int i)
{
    switch (i)
    {
        case 0:
            ui->actionPanel->setScheme(QSint::ActionPanelScheme::defaultScheme());
            break;

        case 1:
            ui->actionPanel->setScheme(QSint::WinXPPanelScheme::defaultScheme());
            break;

        case 2:
            ui->actionPanel->setScheme(QSint::WinXPPanelScheme2::defaultScheme());
            break;

        case 3:
            ui->actionPanel->setScheme(QSint::WinVistaPanelScheme::defaultScheme());
            break;
    }

}
