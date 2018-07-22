#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
<<<<<<< HEAD
=======
    Search *searcher;
    QList<Place*> places;
>>>>>>> 428508d... Built Importer

private slots:
    void on_actionImport_Schools_triggered();

    void on_actionImport_Hospitals_triggered();

    void on_rSlider_valueChanged(int value);

    void on_kmBox_toggled(bool checked);

    void on_milesBox_toggled(bool checked);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
