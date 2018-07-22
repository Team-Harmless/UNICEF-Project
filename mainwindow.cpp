#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>
#include "extractdata.h"
#include <QThread>

double distanceMultiplier = 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    searcher = new Search();
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0,0);
    ui->splitter->setStretchFactor(1,1);
}

MainWindow::~MainWindow()
{
    foreach (Place *pl, places) {
        delete pl;
    }
    delete searcher;
    delete ui;
}

void MainWindow::on_actionImport_Schools_triggered() //Gets full path to file
{
    QString documentsLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Schools File"), documentsLocation, tr("JSON Files (*.json)"));
    if (fileName != "") {
        QList<Place*> schools = Extractor::getSchools(fileName);
        searcher->addData(schools);
        places += schools;
        ui->actionImport_Schools->setEnabled(false);
    }
}

void MainWindow::on_actionImport_Hospitals_triggered()
{
    QString documentsLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Schools File"), documentsLocation, tr("JSON Files (*.json)"));
    if (fileName != "") {
        QList<Place*> healthFacilities = Extractor::getHealthFacilities(fileName);
        searcher->addData(healthFacilities);
        places += healthFacilities;
        ui->actionImport_Hospitals->setEnabled(false);
    }
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

void MainWindow::on_searchBar_textChanged(const QString &arg1)
{
    ui->resultsList->clear();
    if (arg1 == "") return;
    QThread *t = new QThread();
    Worker *w = new Worker();
    w->searcher = searcher;
    w->query = arg1;
    w->moveToThread(t);
    connect(w, SIGNAL(finished()), t, SLOT(quit()));
    connect(w, SIGNAL(finished()), w, SLOT(deleteLater()));
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(t, SIGNAL(started()), w, SLOT(doSearch()));
    connect(w, SIGNAL(addItemToResultsList(QString)), this, SLOT(addItemToRList(QString)));
    t->start();

}

void MainWindow::addItemToRList(QString s)
{
    ui->resultsList->addItem(s);
}


Worker::Worker()
{

}

Worker::~Worker()
{

}

void Worker::doSearch()
{
    QList<Place*> results = searcher->searchForList(query);
    int i = 0;
    foreach(Place *pl, results) {
        if (i > 50) break;
        i++;
        emit(addItemToResultsList(pl->name));
    }
    emit(finished());
}
