#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "search.h"
#include <QList>
#include "place.h"
#include "quadtree.h"
#include <QMessageBox>
#include "context.h"
#include <QListWidgetItem>

class Worker : public QObject {
    Q_OBJECT
public:
    Worker();
    ~Worker();
    Search *searcher;
    QString query;
public slots:
    void doSearch();

signals:
    void finished();
    void addItemToResultsList(Place *s);
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void setMessage(QString s);
    void closeMessage();

private:
    void unitUpdate(double mult);
    Search *searcher;
    Quad *locations;
    Context *context;
    QList<Place*> places;
    QList<Place*> displyedPlaces;
    void applyFilter(QList<Place *> *places);
    QString schoolFile;
    QString hospFile;
    void on_resultsList_currentRowChanged(int currentRow);

private slots:
    void on_actionImport_Schools_triggered();
    void on_actionImport_Hospitals_triggered();
    void on_rSlider_valueChanged(int value);
    void on_kmBox_toggled(bool checked);
    void on_milesBox_toggled(bool checked);
    void on_searchBar_textChanged(const QString &arg1);
    void addItemToRList(Place *s);
    void displayResults();
    void on_schoolsBox_toggled(bool checked);

    void on_healthFacilitiesBox_toggled(bool checked);

    void on_hospitalsBox_toggled(bool checked);

    void on_clinicsBox_toggled(bool checked);

    void on_actionImport_triggered();

    void on_resultsList_itemClicked(QListWidgetItem *);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
