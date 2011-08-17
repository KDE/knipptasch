#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // tab 1 - QGraphicsScene with ScrollPanner
    setupGraphicsScene();

    // tab 2 - QTable with ScrollPanner
    setupTable();

    // tab 3 - QTextBrowser with ScrollPanner
    setupTextBrowser();
}

// tab 1 - QGraphicsScene with ScrollPanner
void MainWindow::setupGraphicsScene()
{
    // create and fill the scene
    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->GraphicsView->setScene(scene);

    for (int i = 0; i < 100; i++)
    {
        QGraphicsRectItem *rect = scene->addRect(
                    qrand() % 2000, qrand() % 2000,
                    qrand() % 400 + 10, qrand() % 400 + 10,
                    QPen(Qt::red), QBrush(Qt::yellow));
    }

    // setup ScrollPanner
    ui->SceneScrollPanner->setBackgroundBrush(QBrush(QColor(Qt::cyan)));
    ui->SceneScrollPanner->setBackgroundPen(QColor(Qt::lightGray));
    ui->SceneScrollPanner->setSliderBrush(QBrush(QColor(Qt::white)));
    ui->SceneScrollPanner->setSliderPen(QColor(Qt::red));
    ui->SceneScrollPanner->setOpacity(1);
    ui->SceneScrollPanner->setSliderOpacity(0.5);

    // connect ScrollPanner to the scene view
    ui->SceneScrollPanner->connectSource(ui->GraphicsView);
}

// tab 2 - QTable with ScrollPanner
void MainWindow::setupTable()
{
    // fill the table
    ui->TableWidget->setRowCount(100);
    ui->TableWidget->setColumnCount(30);

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 30; j++)
        {
            QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(
                     pow(i, j+1)));
            ui->TableWidget->setItem(i, j, newItem);
        }
    }

    // setup ScrollPanner
    ui->TableScrollPanner->setBackgroundBrush(QBrush(QColor(Qt::white)));
    ui->TableScrollPanner->setBackgroundPen(QColor(Qt::gray));
    ui->TableScrollPanner->setSliderBrush(QBrush(QColor(Qt::yellow)));
    ui->TableScrollPanner->setSliderPen(QColor(Qt::lightGray));

    // connect ScrollPanner to the table
    ui->TableScrollPanner->connectSource(ui->TableWidget);
}

// tab 3 - QTextBrowser with ScrollPanner
void MainWindow::setupTextBrowser()
{
    // setup ScrollPanner
    ui->TextBrowserScrollPanner->setBackgroundBrush(QBrush(QColor(Qt::yellow)));
    ui->TextBrowserScrollPanner->setBackgroundPen(QColor(Qt::lightGray));
    ui->TextBrowserScrollPanner->setSliderBrush(QBrush(QColor(Qt::darkGray)));
    ui->TextBrowserScrollPanner->setSliderPen(QColor(Qt::black));

    // connect ScrollPanner to the table
    ui->TextBrowserScrollPanner->connectSource(ui->TextBrowser);
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
