#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>

double distanceMultiplier = 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0,0);
    ui->splitter->setStretchFactor(1,1);
}

MainWindow::~MainWindow()
{
<<<<<<< HEAD
=======
    foreach (Place *pl, places) {
        delete pl;
    }
    delete searcher;
>>>>>>> 428508d... Built Importer
    delete ui;
}

void MainWindow::on_actionImport_Schools_triggered() //Gets full path to file
{
    QString documentsLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Schools File"), documentsLocation, tr("JSON Files (*.json)"));
<<<<<<< HEAD
=======
    if (fileName != "") {
        QList<Place*> schools = Extractor::getSchools(fileName);
        searcher->addData(schools);
        places += schools;
        ui->actionImport_Schools->setEnabled(false);
    }
>>>>>>> 428508d... Built Importer
}

void MainWindow::on_actionImport_Hospitals_triggered()
{
    QString documentsLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Schools File"), documentsLocation, tr("JSON Files (*.json)"));
<<<<<<< HEAD
=======
    if (fileName != "") {
        QList<Place*> healthFacilities = Extractor::getHealthFacilities(fileName);
        searcher->addData(healthFacilities);
        places += healthFacilities;
        ui->actionImport_Hospitals->setEnabled(false);
    }
>>>>>>> 428508d... Built Importer
}

void MainWindow::unitUpdate(double mult) {
    distanceMultiplier = mult;
    ui->radiusLable->setText(QString::number((double)ui->rSlider->value() / 10 * distanceMultiplier, 'g', 2));
}

void MainWindow::on_rSlider_valueChanged(int value)
{
    ui->radiusLable->setText(QString::number((double)value / 10 * distanceMultiplier, 'g', 2));
}

void MainWindow::on_kmBox_toggled(bool checked) { if (checked) unitUpdate(1); }

void MainWindow::on_milesBox_toggled(bool checked) { if (checked) unitUpdate(0.621371); }
