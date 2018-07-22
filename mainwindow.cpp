#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>
#include "extractdata.h"
#include "healthfacility.h"
#include <QThread>

double distanceMultiplier = 1;
QThread *searchThread = NULL;

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
        qDebug() << "Imported" << schools.count() << "schools";
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
    if (searchThread != NULL) searchThread->blockSignals(true);
    ui->resultsList->clear();
    displyedPlaces.clear();
    if (arg1 == "") return;
    QThread *t = new QThread();
    Worker *w = new Worker();
    w->searcher = searcher;
    w->query = arg1;
    w->moveToThread(t);
    connect(w, SIGNAL(finished()), t, SLOT(quit()));
    connect(w, SIGNAL(finished()), this, SLOT(displayResults()));
    connect(w, SIGNAL(finished()), w, SLOT(deleteLater()));
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(t, SIGNAL(started()), w, SLOT(doSearch()));
    connect(w, SIGNAL(addItemToResultsList(Place*)), this, SLOT(addItemToRList(Place*)));
    t->start();
    searchThread = t;

}

QList<Place*> MainWindow::applyFilter(QList<Place*> places) {
    QList<Place*> ret = places;
    if (!ui->schoolsBox->isChecked())
        for(int i = ret.count() -1; i >= 0; i--)
            if (ret.at(i)->classType == Place::Schl) ret.removeAt(i);
    if (!ui->healthFacilitiesBox->isChecked())
        for(int i = ret.count() -1; i >= 0; i--)
            if (ret.at(i)->classType == Place::HlthFac) ret.removeAt(i);
    if (!ui->clinicsBox->isChecked())
        for(int i = ret.count() -1; i >= 0; i--)
            if (ret.at(i)->classType == Place::HlthFac &&
                    ((HealthFacility*)ret.at(i))->type.toLower() == "clinic") ret.removeAt(i);
    if (!ui->hospitalsBox->isChecked())
        for(int i = ret.count() -1; i >= 0; i--)
            if (ret.at(i)->classType == Place::HlthFac &&
                    ((HealthFacility*)ret.at(i))->type.toLower() == "hospital") ret.removeAt(i);
    return ret;
}

void MainWindow::addItemToRList(Place* s)
{
    searchThread = NULL;
    displyedPlaces << s;
}

void MainWindow::displayResults()
{
    ui->resultsList->clear();
    QList<Place*> dis = applyFilter(displyedPlaces);
    foreach(Place *pl, dis)
        ui->resultsList->addItem(pl->name);
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
    foreach(Place *pl, results)
        emit(addItemToResultsList(pl));
    emit(finished());
}

void MainWindow::on_schoolsBox_toggled(bool)
{
    displayResults();
}

void MainWindow::on_healthFacilitiesBox_toggled(bool)
{
    displayResults();
}

void MainWindow::on_hospitalsBox_toggled(bool)
{
    displayResults();
}

void MainWindow::on_clinicsBox_toggled(bool)
{
    displayResults();
}
