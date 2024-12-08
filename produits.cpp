#include "produits.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>           // For showing message boxes (QMessageBox)
          // Pour qDebug() et le débogage
#include <QString>         // Pour QString
#include <QDate>           // Pour QDate
       // Pour les requêtes SQL avec QSqlQuery
#include <QSqlError>       // Pour gérer les erreurs SQL


// Default constructor
produits::produits() {}

// Constructor to create a new produit
produits::produits(QString QUANTITE, QString EMPLACEMENT, QString DATE_ENTREE, QString DATE_ESPIRATION)
{
    this->QUANTITE = QUANTITE;
    this->EMPLACEMENT = EMPLACEMENT;
    this->DATE_ENTREE = DATE_ENTREE;
    this->DATE_ESPIRATION = DATE_ESPIRATION;
}


// Constructor to modify an existing produits by ID

produits::produits(int ID_PROD, QString QUANTITE, QString EMPLACEMENT, QString DATE_ENTREE, QString DATE_ESPIRATION)
{
    this->ID_PROD = ID_PROD;
    this->QUANTITE = QUANTITE;
    this->EMPLACEMENT = EMPLACEMENT;
    this->DATE_ENTREE = DATE_ENTREE;
    this->DATE_ESPIRATION = DATE_ESPIRATION;
}

// Add a new produit to the database
void produits::ajouter()
{
    // Créer une requête SQL pour insérer un nouveau patient
    QSqlQuery query;

    // Préparer la requête d'insertion
    query.prepare("INSERT INTO PRODUITS (QUANTITE, EMPLACEMENT, DATE_ENTREE, DATE_ESPIRATION) "
                  "VALUES (:QUANTITE, :EMPLACEMENT, :DATE_ENTREE, :DATE_ESPIRATION)");

    // Lier les valeurs
    query.bindValue(":QUANTITE", QUANTITE);
    query.bindValue(":EMPLACEMENT", EMPLACEMENT);
    query.bindValue(":DATE_ENTREE", DATE_ENTREE);
    query.bindValue(":DATE_ESPIRATION", DATE_ESPIRATION);


    // Afficher les valeurs avant d'exécuter la requête pour le débogage
    qDebug() << "Ajout du produit avec les valeurs :";
    qDebug() << "QUANTITE: " << QUANTITE;
    qDebug() << "EMPLACEMENT: " << EMPLACEMENT;
    qDebug() << "DATE_ENTREE: " << DATE_ENTREE;
    qDebug() << "DATE_ESPIRATION: " << DATE_ESPIRATION;


    // Exécuter la requête
    if (!query.exec()) {
        // Si l'exécution échoue, afficher l'erreur
        qDebug() << "Erreur lors de l'ajout du produit: " << query.lastError().text();
    } else {
        qDebug() << "Produit ajouté avec succès!";
    }
}
// Display all produits in a model
QSqlQueryModel* produits::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PRODUITS");  // Vérifiez bien la syntaxe ici
    return model;
}



// Delete a produits by ID
bool produits::supp(int ID_PROD)
{
    QSqlQuery query;
    query.prepare("DELETE FROM PRODUITS WHERE ID_PROD = :id");
    query.bindValue(":id", ID_PROD);
    return query.exec();
}

// Modify an existing produit by ID
bool produits::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE PRODUITS SET QUANTITE = :QUANTITE, EMPLACEMENT= :EMLPACEMENT, DATE_ENTREE= :DATE_ENTREE,DATE_ESPIRATION= : DATE_ESPIRATION :WHERE ID_PROD = :id");
            query.bindValue(":id", ID_PROD);
            query.bindValue(":QUANTITE", QUANTITE);
            query.bindValue(":EMPLACEMENT", EMPLACEMENT);
            query.bindValue(":DATE_ENTREE", DATE_ENTREE);
            query.bindValue(":DATE_ESPIRATION", DATE_ESPIRATION);


    return query.exec();  // returns true if successful, false if failed
}


// Search for a produits by ID
bool produits::recherche(int ID_PROD)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM PRODUITS WHERE ID_PROD = :id");
    query.bindValue(":id", ID_PROD);

    if (query.exec() && query.next()) {
        return true;
    } else {
        qDebug() << "Error while searching for patient by ID:" << query.lastError().text();
        return false;
    }
}



#include <QSqlError> // Pour accéder aux erreurs SQL

QSqlQueryModel* produits::tri_par_id_croissant()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PRODUITS ORDER BY ID_PROD ASC");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur dans tri_par_id_croissant:" << model->lastError().text();
    }

    return model;
}

// Trier les produits par ID décroissant
QSqlQueryModel* produits::tri_par_id_decroissant()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PRODUITS ORDER BY ID_PROD DESC");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur dans tri_par_id_decroissant:" << model->lastError().text();
    }

    return model;
}

// Trier les produits par NOM croissant
QSqlQueryModel* produits::tri_par_QUANTITE_croissant()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PRODUITS ORDER BY QUANTITE ASC, ID_PROD ASC");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur dans tri_par_nom_croissant:" << model->lastError().text();
    }

    return model;
}


QSqlQueryModel* produits::tri_par_QUANTITE_decroissant()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PRODUITS ORDER BY QUANTITE DESC, ID_PROD ASC");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur dans tri_par_QUANTITE_decroissant:" << model->lastError().text();
    }

    return model;
}


QSqlQueryModel* produits::tri_par_EMPLACEMENT_croissant()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PRODUITS ORDER BY EMPLACEMENT ASC, ID_PROD ASC");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur dans tri_par_EMPLACEMENT_croissant:" << model->lastError().text();
    }

    return model;
}


QSqlQueryModel* produits::tri_par_EMPLACEMENT_decroissant()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PRODUITS ORDER BY EMPLACEMENT DESC, ID_PROD ASC");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur dans tri_par_EMPLACEMENT_decroissant:" << model->lastError().text();
    }

    return model;
}
