#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "search.h"
#include <QList>
#include "place.h"

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
    void addItemToResultsList(QString s);
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

private:
    void unitUpdate(double mult);
    Search *searcher;
    QList<Place*> places;

private slots:
    void on_actionImport_Schools_triggered();

    void on_actionImport_Hospitals_triggered();

    void on_rSlider_valueChanged(int value);

    void on_kmBox_toggled(bool checked);

    void on_milesBox_toggled(bool checked);

    void on_searchBar_textChanged(const QString &arg1);

    void addItemToRList(QString s);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
