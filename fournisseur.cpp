#include "fournisseur.h"
#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTextDocument>
#include <QString>
#include <QMessageBox>
#include <QPainter>
#include <QDateTime>
#include <QPrinter>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QPageSize>
#include <QMap>
#include <QSqlError>








fournisseur::fournisseur()
{
}

fournisseur::fournisseur(QString id, QString nom, QString prenom, QString adresse ,QString tel , QString email)
{
    this->id=id;
    this->nom=nom;
    this->prenom=prenom;
    this->adresse=adresse;
    this->tel=tel;
    this->email=email;
}

void fournisseur::setnom(QString n) { this->nom = n; }
void fournisseur::setprenom(QString n) { this->prenom = n; }
void fournisseur::setadresse(QString n) { this->adresse = n; }
void fournisseur::settel(QString n) { this->tel = n; }
void fournisseur::setemail(QString n) { this->email = n; }

QString fournisseur::get_id() { return id; }
QString fournisseur::get_nom() { return nom; }
QString fournisseur::get_prenom() { return prenom; }
QString fournisseur::get_adresse() { return adresse; }
QString fournisseur::get_tel() { return tel; }
QString fournisseur::get_email() { return email; }

bool fournisseur::ajouter()
{
    QSqlQuery query;

    query.prepare("INSERT INTO fournisseur (ID_F, NOM_F, PRENOM_F, ADRESSE_F, TEL_F, EMAIL_F) "
                      "VALUES (:id, :nom, :prenom, :adresse, :tel, :email)");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":adresse", adresse);
    query.bindValue(":tel", tel);
    query.bindValue(":email", email);

    return query.exec();
}

QSqlQueryModel* fournisseur::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID_F, NOM_F, PRENOM_F, ADRESSE_F, TEL_F, EMAIL_F FROM fournisseur");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Tél"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Email"));

    return model;
}

bool fournisseur::supprimer(QString id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM fournisseur WHERE ID_F=:id");
    query.bindValue(":id", id);
    return query.exec();
}

bool fournisseur::modifier(QString id)
{
    QSqlQuery query;

    query.prepare("UPDATE fournisseur SET NOM_F = :nom, PRENOM_F = :prenom, ADRESSE_F = :adresse, "
                      "TEL_F = :tel, EMAIL_F = :email WHERE ID_F = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":adresse", adresse);
    query.bindValue(":tel", tel);
    query.bindValue(":email", email);

    return query.exec();
}

bool fournisseur::recherchee(QString nom, QSqlQueryModel*& model)
{
    // Vérification si le IDF est vide
    if (nom.isEmpty()) {
        QMessageBox::information(nullptr, "Information", "Le champ nom est vide.");
        return false;
    }
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM fournisseur WHERE nom = :nom");
    checkQuery.bindValue(":nom", nom);
    if (!checkQuery.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la vérification du nom.");
        return false;
    }
    checkQuery.next();
    int count = checkQuery.value(0).toInt();
    if (count == 0) {
        QMessageBox::information(nullptr, "Information", "Le nom spécifié n'existe pas dans la base de données.");
        return false;
    }
    model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM fournisseur WHERE nom = :nom");
    query.bindValue(":nom", nom);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la récupération des données.");
        return false;
    }
    model->setQuery(std::move(query));
    return true;
}
bool fournisseur::exporterPDF(const QString& fileName, QSqlQueryModel* model)
{
    qDebug() << "exporterPDF function called with fileName:" << fileName;  // Debug line

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A3));
    printer.setOutputFileName(fileName);

    QPainter painter(&printer);
    if (!painter.isActive()) {
        QMessageBox::warning(nullptr, "Erreur", "Impossible de dessiner sur l'imprimante.");
        return false;
    }

    QTextDocument doc;

    // Base HTML content with themed styling
    QString html = "<html><head><style>"
                   "body { font-family: Arial, sans-serif; }"
                   "h1, h2 { color: #B22222; text-align: center; }"
                   "table { border-collapse: collapse; width: 100%; margin: 20px auto; }"
                   "th, td { border: 1px solid #ddd; text-align: center; padding: 8px; font-size: 100%; }"
                   "th { background-color: #B22222; color: white; }"
                   "tr:nth-child(even) { background-color: #f2f2f2; }"
                   "tr:hover { background-color: #ddd; }"
                   "footer { text-align: center; color: grey; margin-top: 50px; }"
                   "</style></head><body>";

    html += "<h1>Rapport des Fournisseurs</h1>";
    html += "<h2>Liste des Fournisseurs</h2><table>";

    // Add column headers
    html += "<tr>";
    for (int col = 0; col < model->columnCount(); ++col) {
        html += "<th>" + model->headerData(col, Qt::Horizontal).toString() + "</th>";
    }
    html += "</tr>";

    // Add rows of data
    for (int row = 0; row < model->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 0; col < model->columnCount(); ++col) {
            html += "<td>" + model->data(model->index(row, col)).toString() + "</td>";
        }
        html += "</tr>";
    }
    html += "</table>";

    // Add Statistics Section
    QMap<QString, int> stats = getCountByAdresse();
    if (!stats.isEmpty()) {
        html += "<h2>Statistiques par Adresse</h2><table>";
        html += "<tr><th>Adresse</th><th>Nombre</th></tr>";

        for (auto it = stats.begin(); it != stats.end(); ++it) {
            html += "<tr><td>" + it.key() + "</td><td>" + QString::number(it.value()) + "</td></tr>";
        }
        html += "</table>";
    }

    // Add Commande Section
    QSqlQuery query;
    query.prepare("SELECT F.ID_F, F.NOM_F, F.PRENOM_F, C.ID_COMMANDE, C.DATE_COMMANDE, C.TOTAL "
                  "FROM fournisseur F "
                  "JOIN commande C ON F.ID_F = C.ID_F");
    if (query.exec()) {
        html += "<h2>Commandes Liées aux Fournisseurs</h2><table>";
        html += "<tr><th>ID Fournisseur</th><th>Nom</th><th>Prénom</th><th>ID Commande</th><th>Date Commande</th><th>Total</th></tr>";

        while (query.next()) {
            html += "<tr>";
            html += "<td>" + query.value("ID_F").toString() + "</td>";
            html += "<td>" + query.value("NOM_F").toString() + "</td>";
            html += "<td>" + query.value("PRENOM_F").toString() + "</td>";
            html += "<td>" + query.value("ID_COMMANDE").toString() + "</td>";
            html += "<td>" + query.value("DATE_COMMANDE").toString() + "</td>";
            html += "<td>" + query.value("TOTAL").toString() + "</td>";
            html += "</tr>";
        }
        html += "</table>";
    } else {
        qDebug() << "Failed to fetch commandes: " << query.lastError().text();
    }

    // Add Footer
    html += "<footer>Rapport généré automatiquement le " + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm") + "</footer>";

    // Finalize HTML
    html += "</body></html>";

    // Set and Render HTML Content
    doc.setHtml(html);
    doc.drawContents(&painter);

    return true;
}



QSqlQueryModel* fournisseur::trierParNom()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM fournisseur ORDER BY NOM_F ASC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("tél"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Email"));

    return model;
}

QMap<QString, int> fournisseur::getCountByAdresse()
{
    QMap<QString, int> addressCounts;
    QSqlQuery query;

    // Query to count fournisseurs per adresse
    query.prepare("SELECT ADRESSE_F, COUNT(*) as count FROM fournisseur GROUP BY ADRESSE_F");

    if (query.exec()) {
        while (query.next()) {
            QString adresse = query.value(0).toString();
            int count = query.value(1).toInt();
            addressCounts[adresse] = count;
        }
    } else {
        qDebug() << "Error fetching statistics: " << query.lastError().text();
    }

    return addressCounts;
}

bool fournisseur::searchById(const QString &id) const
{
    QSqlQuery query;
    query.prepare("SELECT * FROM fournisseur WHERE ID_F = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        if (query.next()) {
            // Retrieve information from the query result
            QString nom = query.value("NOM_F").toString();
            QString prenom = query.value("PRENOM_F").toString();
            QString adresse = query.value("ADRESSE_F").toString();
            QString tel = query.value("TEL_F").toString();
            QString email = query.value("EMAIL_F").toString();

            // Display information in a message box
            QMessageBox msgBox;
            msgBox.setWindowTitle("Fournisseur trouvé");
            msgBox.setText("Détails du Fournisseur:\n\n"
                           "ID: " + id + "\n" +
                           "Nom: " + nom + "\n" +
                           "Prenom: " + prenom + "\n" +
                           "Adresse: " + adresse + "\n" +
                           "Téléphone: " + tel + "\n" +
                           "Email: " + email);
            msgBox.exec();
            return true;
        } else {
            // Show a message if the ID was not found
            QMessageBox::warning(nullptr, "Non trouvé", "Aucun fournisseur trouvé avec cet ID : " + id);
            return false;
        }
    } else {
        // Handle query execution error
        qDebug() << "Database error: " << query.lastError().text();
        QMessageBox::critical(nullptr, "Database Error", "Failed to retrieve fournisseur information.");
        return false;
    }
}

bool fournisseur::searchByName(const QString &name) const
{
    QSqlQuery query;
    query.prepare("SELECT * FROM fournisseur WHERE NOM_F = :name");
    query.bindValue(":name", name);

    if (query.exec()) {
        if (query.next()) {
            // Prepare to store details of all matching results
            QString details;
            do {
                QString id = query.value("ID_F").toString();
                QString prenom = query.value("PRENOM_F").toString();
                QString adresse = query.value("ADRESSE_F").toString();
                QString tel = query.value("TEL_F").toString();
                QString email = query.value("EMAIL_F").toString();

                // Append the details of the current record to the details string
                details += "ID: " + id + "\n"
                           "Nom: " + name + "\n"
                           "Prenom: " + prenom + "\n"
                           "Adresse: " + adresse + "\n"
                           "Téléphone: " + tel + "\n"
                           "Email: " + email + "\n\n";
            } while (query.next()); // Loop through all matching results

            // Display the results in a message box
            QMessageBox msgBox;
            msgBox.setWindowTitle("Fournisseurs trouvés");
            msgBox.setText("Détails des fournisseurs:\n\n" + details);
            msgBox.exec();
            return true;
        } else {
            // Show a message if no matches are found
            QMessageBox::warning(nullptr, "Non trouvé", "Aucun fournisseur trouvé avec ce nom : " + name);
            return false;
        }
    } else {
        // Handle query execution error
        qDebug() << "Database error: " << query.lastError().text();
        QMessageBox::critical(nullptr, "Database Error", "Failed to retrieve fournisseur information.");
        return false;
    }
}






