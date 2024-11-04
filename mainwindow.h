#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "vehicule.h"
#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //void on_buttonAjouter_clicked();
    void on_buttonSupprimer_clicked();
    void on_RechercherID_clicked();


    void on_pushButtonAjouter_clicked();

    void on_Supprimer_clicked();

    void on_Rechercher_clicked();
    void on_sauvegarde_clicked();

private:
    Ui::MainWindow *ui;
    Vehicule Vtmp;
};

#endif // MAINWINDOW_H
