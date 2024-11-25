#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "fournisseur.h"
#include <QMainWindow>
#include "connection.h"
#include <QCalendarWidget>
#include "calender.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadFournisseurData();


private slots:
    QString controlSaisie();
    void on_ajoutBTN_clicked();
    void on_modifierBTN_clicked();
    //void on_modifierBTN_clickedkk();
    void on_supprimerBTN_clicked();
    void onRowSelected(const QModelIndex &index);  
    void on_pdfIN_clicked();

    //void on_pushButton_clicked();

    //void on_lineEdit_editingFinished();

    //void on_lineEdit_textChanged(const QString &arg1);

    void on_rechercheIN_clicked();
    void showStatistics();
    void on_statisticsButton_clicked();
    void on_sortCheck_clicked();
    void populateCalendarWithData(QCalendarWidget *calendar);
    void logHistory(const QString &message);


    //void on_calendrier_clicked();

    void on_calendrier_clicked();
    void loadLogFileContent();

    void on_rechercheIN_2_clicked();

    void on_rechercheIN_3_clicked();

    void on_refresh_clicked();

private:
    Ui::MainWindow *ui;
    fournisseur F;

    Calender *calenderWidget;

};

#endif // MAINWINDOW_H
