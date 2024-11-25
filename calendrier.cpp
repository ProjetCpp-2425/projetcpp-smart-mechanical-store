#include "calendrier.h"
#include "ui_calendrier.h"  // Ensure this is included
#include "mainwindow.h"     // Include header, not the .cpp file
#include <QSqlQuery>
#include <QDate>
#include <QTextCharFormat>
#include <QMap>
#include <QMessageBox>

calendrier::calendrier(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::calendrier)
{
    ui->setupUi(this);
}

calendrier::~calendrier()
{
    delete ui;
}

void MainWindow::populateCalendarWithData(QCalendarWidget *calendar)
{
    QMap<QDate, QString> commandes;
    QSqlQuery query("SELECT DATE_C, ID_COMMANDE, ID_F FROM COMMANDE");
    if (!query.exec()) {
        QMessageBox::warning(this, "Database Error", query.lastError().text());
        return;
    }

    while (query.next()) {
        QDate date = QDate::fromString(query.value("DATE_C").toString(), "yyyy-MM-dd");
        QString idCommande = query.value("ID_COMMANDE").toString();
        QString idFournisseur = query.value("ID_F").toString();

        QString info = QString("ID Commande: %1\nID Fournisseur: %2").arg(idCommande).arg(idFournisseur);
        commandes[date] = info;
    }

    // Set tooltips for each date with commande information
    for (auto it = commandes.begin(); it != commandes.end(); ++it) {
        QDate date = it.key();
        QString info = it.value();

        // Only display tooltip if the date is valid and within the current month displayed
        if (calendar->dateTextFormat(date).isValid()) {
            QTextCharFormat format;
            format.setToolTip(info);
            calendar->setDateTextFormat(date, format);
        }
    }
}
