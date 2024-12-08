#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");
    db.setUserName("yassine");
    db.setPassword("kaabi123");

    if (db.open()) {
        test = true;
        qDebug() << "Connexion réussie à la base de données.";
    } else {
        qDebug() << "Erreur de connexion à la base de données:" << db.lastError().text();
    }

    return test;
}

