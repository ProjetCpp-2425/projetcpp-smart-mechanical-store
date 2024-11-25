#include "calender.h"
#include "ui_calender.h"
#include <QSqlQuery>
#include <QDate>
#include <QTextCharFormat>
#include <QMap>
#include <QMessageBox>
#include <QSqlError>

Calender::Calender(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Calender)
{
    ui->setupUi(this);

    // Connect the clicked signal to a slot
    connect(ui->calendrier, &QCalendarWidget::clicked, this, &Calender::onDateSelected);
}

Calender::~Calender()
{
    delete ui;
}

void Calender::populateCalendarWithData(QCalendarWidget *calendar)
{
    QMap<QDate, QString> commande;
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
        commande[date] = info;
    }

    for (auto it = commande.begin(); it != commande.end(); ++it) {
        QDate date = it.key();
        QString info = it.value();

        QTextCharFormat format;
        format.setToolTip(info); // Add tooltip to the date
        format.setBackground(Qt::yellow); // Highlight the date in the calendar
        calendar->setDateTextFormat(date, format);
    }
}

void Calender::onDateSelected(const QDate &date)
{
    QSqlQuery query;
    query.prepare("SELECT C.ID_COMMANDE, C.ID_F, F.NAME, F.ADDRESS FROM COMMANDE C "
                  "JOIN FOURNISSEUR F ON C.ID_F = F.ID WHERE C.DATE_C = :date");
    query.bindValue(":date", date.toString("yyyy-MM-dd"));

    if (!query.exec()) {
        QMessageBox::warning(this, "Database Error", query.lastError().text());
        return;
    }

    QString message;
    while (query.next()) {
        QString idCommande = query.value("ID_COMMANDE").toString();
        QString idFournisseur = query.value("ID_F").toString();
        QString fournisseurName = query.value("NAME").toString();
        QString fournisseurAddress = query.value("ADDRESS").toString();

        message += QString("ID Commande: %1\nID Fournisseur: %2\nFournisseur Name: %3\nFournisseur Address: %4\n\n")
                        .arg(idCommande)
                        .arg(idFournisseur)
                        .arg(fournisseurName)
                        .arg(fournisseurAddress);
    }

    if (message.isEmpty()) {
        QMessageBox::information(this, "No Data", "No commandes found for the selected date.");
    } else {
        QMessageBox::information(this, "Commandes Information", message);
    }
}
