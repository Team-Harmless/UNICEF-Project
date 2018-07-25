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
QThread *importThread = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    searcher = new Search();
    locations = NULL;
    searchThread = new QThread();
    importThread = new QThread();
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0,0);
    ui->splitter->setStretchFactor(1,1);
}

MainWindow::~MainWindow()
{
    foreach (Place *pl, places) {
        delete pl;
    }
    while (searchThread->isRunning()) ;;
    delete searchThread;
    delete searcher;
    delete ui;
}

void MainWindow::on_actionImport_Schools_triggered() //Gets full path to file
{
    QString documentsLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
    schoolFile = QFileDialog::getOpenFileName(this, tr("Open Schools File"), documentsLocation, tr("JSON Files (*.json)"));
}

void MainWindow::on_actionImport_Hospitals_triggered()
{
    QString documentsLocation = QStandardPaths::locate(QStandardPaths::DocumentsLocation, QString(), QStandardPaths::LocateDirectory);
    hospFile = QFileDialog::getOpenFileName(this, tr("Open Hospitals"), documentsLocation, tr("JSON Files (*.json *.geojson)"));
}

void MainWindow::unitUpdate(double mult) {
    distanceMultiplier = mult;
    ui->radiusLable->setText(QString::number((double)ui->rSlider->value() / 10 * distanceMultiplier, 'g', ui->rSlider->value() > 100 ? 3 : 2));
}

void MainWindow::on_rSlider_valueChanged(int value)
{
    ui->radiusLable->setText(QString::number((double)value / 10 * distanceMultiplier, 'g', value > 100 ? 3 : 2));
}

void MainWindow::on_kmBox_toggled(bool checked) { if (checked) unitUpdate(1); }

void MainWindow::on_milesBox_toggled(bool checked) { if (checked) unitUpdate(0.621371); }

void MainWindow::on_searchBar_textChanged(const QString &arg1)
{
    qDebug() << "search";
    while(searchThread->isRunning()) ;;
    ui->resultsList->clear();
    displyedPlaces.clear();
    Worker *w = new Worker();
    w->searcher = searcher;
    w->query = arg1;
    w->moveToThread(searchThread);
    connect(w, SIGNAL(finished()), searchThread, SLOT(quit()));
    connect(w, SIGNAL(finished()), this, SLOT(displayResults()));
    connect(w, SIGNAL(finished()), w, SLOT(deleteLater()));
    connect(searchThread, SIGNAL(started()), w, SLOT(doSearch()));
    connect(w, SIGNAL(addItemToResultsList(Place*)), this, SLOT(addItemToRList(Place*)));
    searchThread->start();
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

void MainWindow::on_actionImport_triggered()
{
    QMessageBox msgBox;
    ui->resultsList->clear();
    displyedPlaces.clear();

    delete searcher; searcher = new Search();
    if(locations != NULL) delete locations;
    ui->actionImport_Schools->setEnabled(false);
    ui->actionImport_Hospitals->setEnabled(false);

    if (hospFile != "") {
        QList<Place*> healthFacilities = Extractor::getHealthFacilities(hospFile);
        places += healthFacilities;
    }
    if (schoolFile != "") {
        QList<Place*> schools = Extractor::getHealthFacilities(schoolFile);
        places += schools;
    }
    searcher->addData(places);
    locations = new Quad(places.toSet());
    ui->actionImport->setEnabled(false);
}
