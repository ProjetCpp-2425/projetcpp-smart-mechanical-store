#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "produits.h"
#include <QMessageBox>           // For showing message boxes (QMessageBox)
#include <QSqlQuery>             // For executing SQL queries (QSqlQuery)
#include <QSqlQueryModel>        // For working with SQL query models (QSqlQueryModel)
#include <QSqlDatabase>          // For setting up the database connection (QSqlDatabase)
#include <QSqlError>             // For handling SQL errors (QSqlError)
#include <QWidget>               // For QWidget class and its derivatives (QMainWindow)
#include <QPieSeries>            // For creating pie charts (QPieSeries)
#include <QPieSlice>             // For creating pie slices in charts (QPieSlice)
#include <QChart>                // For charts (QChart)
#include <QChartView>            // For displaying charts (QChartView)
#include <QColor>                // For using colors (QColor)
#include <QList>                 // For working with lists (QList)
#include <QDebug>                // For debugging (QDebug)
#include <QPainter>              // For drawing (QPainter)
#include <QFileDialog>           // For file dialog to save the PDF
#include <QPdfWriter>            // For writing the PDF file
#include <QFont>                 // For setting the font in the PDF
#include <QPageSize>             // For setting the page size (e.g., A3)
#include <QVector>               // For using QVector to define column widths
#include <QStringList>           // For using QStringList to define column titles
#include <numeric>               // For using std::accumulate to calculate column widths
#include <QSqlRecord>            // For handling SQL records
#include <QNetworkAccessManager> // For network access (QNetworkAccessManager)
#include <QNetworkRequest>       // For creating network requests (QNetworkRequest)
#include <QNetworkReply>         // For handling network replies (QNetworkReply)
#include <QUrl>                  // For working with URLs (QUrl)
#include <QUrlQuery>             // For URL queries (QUrlQuery)
#include <QSqlError>
#include <QMessageBox>
#include <QPixmap>
#include <QSerialPort>
#include <QSerialPortInfo>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)   // Initialisation de l'interface utilisateur
{
    ui->setupUi(this);

    ui->comboBox->addItem("EMPLACEMENT");
    ui->comboBox->addItem("QUANTITE");

       ui->comboBox->addItem("ID_PROD");// Configuration de l'interface utilisateur


       serial.setPortName("COM5"); // Remplacez COM3 par le port réel de votre Arduino
               serial.setBaudRate(QSerialPort::Baud9600);
               serial.setDataBits(QSerialPort::Data8);
               serial.setParity(QSerialPort::NoParity);
               serial.setStopBits(QSerialPort::OneStop);
               serial.setFlowControl(QSerialPort::NoFlowControl);

               if (!serial.open(QIODevice::WriteOnly)) {
                   qDebug() << "Erreur d'ouverture du port série:" << serial.errorString();
                   return;
               }

     }

MainWindow::~MainWindow() {
    delete ui;
    serial.close(); // Fermer le port série à la fermeture de l'application

}







#include <QDebug>          // Pour qDebug() et le débogage
#include <QString>         // Pour QString
#include <QDate>           // Pour QDate
#include <QSqlQuery>       // Pour les requêtes SQL
#include <QSqlError>       // Pour gérer les erreurs SQL

void MainWindow::on_ajouter_clicked()
{
    QString QUANTITE_STR = ui->c->text();  // Get quantity as a string
    QString EMPLACEMENT = ui->d->text();   // Get location
    QString DATE_ENTREE_STR = ui->e->text(); // Get entry date
    QString DATE_ESPIRATION_STR = ui->f->text(); // Get expiration date
    produits p;
    bool isInt;
    int QUANTITE = QUANTITE_STR.toInt(&isInt);
    if (!isInt) {
        QMessageBox::warning(this, "Erreur", "La QUANTITE entrée est invalide !");
        return;
    }

    // Test multiple date formats for ENTRY date
    QDate DATE_ENTREE = QDate::fromString(DATE_ENTREE_STR, "yyyy-MM-dd");
    if (!DATE_ENTREE.isValid()) {
        DATE_ENTREE = QDate::fromString(DATE_ENTREE_STR, "dd/MM/yyyy");
    }
    if (!DATE_ENTREE.isValid()) {
        QMessageBox::warning(this, "Erreur", "La DATE_ENTREE est invalide ! Assurez-vous qu'elle est au format yyyy-MM-dd ou dd/MM/yyyy.");
        return;
    }

    QDate DATE_ESPIRATION = QDate::fromString(DATE_ESPIRATION_STR, "yyyy-MM-dd");
    if (!DATE_ESPIRATION.isValid()) {
        DATE_ESPIRATION = QDate::fromString(DATE_ESPIRATION_STR, "dd/MM/yyyy");
    }
    if (!DATE_ESPIRATION.isValid()) {
        QMessageBox::warning(this, "Erreur", "La DATE_ESPIRATION est invalide ! Assurez-vous qu'elle est au format yyyy-MM-dd ou dd/MM/yyyy.");
        return;
    }

    // Prepare the SQL query
    QSqlQuery query;
    query.prepare("INSERT INTO PRODUITS (QUANTITE, EMPLACEMENT, DATE_ENTREE, DATE_ESPIRATION) "
                  "VALUES (:QUANTITE, :EMPLACEMENT, :DATE_ENTREE, :DATE_ESPIRATION)");

    query.bindValue(":QUANTITE", QUANTITE);
    query.bindValue(":EMPLACEMENT", EMPLACEMENT);
    query.bindValue(":DATE_ENTREE", DATE_ENTREE.toString("yyyy-MM-dd"));
    query.bindValue(":DATE_ESPIRATION", DATE_ESPIRATION.toString("yyyy-MM-dd"));

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Produit ajouté avec succès.");
        ui->tableView->setModel(p.afficher());  // Refresh the table view
        sendTotalQuantityToArduino();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du produit : " + query.lastError().text());
    }
}


void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    // Charger automatiquement les données dans le tableView
    produits p;
    ui->tableView->setModel(p.afficher());

    // Send the total quantity to Arduino when the window is shown
       sendTotalQuantityToArduino();
}

void MainWindow::on_supprimer_clicked()
{
    produits p;
    int ID_PROD = ui->b->text().toInt();
    if (p.supp(ID_PROD))
    {
        ui->tableView->setModel(p.afficher());
        QMessageBox::information(this, "Succès", "Patient supprimé avec succès.");
        sendTotalQuantityToArduino();

    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression du patient.");
    }
}


void MainWindow::on_modifier_clicked()
{
      QString id = ui->b->text(); // ID of the patient to modify
    QString QUANTITE = ui->c->text();
    QString EMPLACEMENT = ui->d->text();
    QString DATE_ENTREE = ui->e->text();
    QString DATE_ESPIRATION = ui->f->text();

    sendTotalQuantityToArduino();

}


void MainWindow::on_recherche_clicked()
{
    QString critere = ui->comboBox->currentText();
    QString valeurRecherche = ui->b_2->text();

    if (valeurRecherche.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une valeur de recherche.");
        return;
    }

    QString colonne;
    if (critere == "QUANTITE") {
        colonne = "QUANTITE";
    } else if (critere == "EMPLACEMENT") {
        colonne = "EMPLACEMENT";
    } else if (critere == "ID_PROD") {
        colonne = "ID_PROD";
    } else {
        QMessageBox::warning(this, "Erreur", "Critère de recherche non valide.");
        return;
    }

    QSqlQuery query;
    if (colonne == "ID") {
        query.prepare("SELECT * FROM PRODUITS WHERE ID_PROD = :valeur");
        query.bindValue(":valeur", valeurRecherche.toInt());
    } else {
        query.prepare(QString("SELECT * FROM PRODUITS WHERE %1 LIKE :valeur").arg(colonne));
        query.bindValue(":valeur", "%" + valeurRecherche + "%");
    }

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'exécution de la recherche : " + query.lastError().text());
        return;
    }

    QSqlQueryModel *model = new QSqlQueryModel(this);

    // Use std::move to pass the query by move instead of copy
    model->setQuery(std::move(query));

    ui->tableView->setModel(model);

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Résultats", "Aucun produits trouvé avec ce critère.");
    }
}



void MainWindow::on_statistique_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT EMPLACEMENT, COUNT(*) FROM PRODUITS GROUP BY EMPLACEMENT");

    if (!query.exec())
    {
        qDebug() << "Erreur lors de l'exécution de la requête.";
        return;
    }

    QPieSeries *series = new QPieSeries();
    while (query.next())
    {
        QString maladie = query.value(0).toString();
        int count = query.value(1).toInt();
        series->append(maladie, count);
    }

    QList<QColor> colors = {
        QColor("#f44336"), QColor("#9c27b0"), QColor("#3f51b5"), QColor("#03a9f4"),
        QColor("#009688"), QColor("#8bc34a"), QColor("#ffeb3b"), QColor("#ff9800"),
        QColor("#795548")
    };

    int colorIndex = 0;
    foreach (QPieSlice *slice, series->slices())
    {
        QString label = QString("%1 (%2%)").arg(slice->label()).arg(100 * slice->percentage(), 0, 'f', 1);
        slice->setLabel(label);

        if (colorIndex < colors.size())
        {
            slice->setColor(colors[colorIndex]);
            colorIndex++;
        }
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques des EMPLACEMENT");

    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(640, 480);
    chartView->show();
}

// Function to export patient data to PDF

// Function to show statistics of patients by chronic disease

void MainWindow::on_pdf_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF File", "", "PDF Files (*.pdf)");
                if (fileName.isEmpty()) {
                    return; // User canceled the operation
                }

                // Create a PDF file
                QPdfWriter pdfWriter(fileName);
                pdfWriter.setPageSize(QPageSize(QPageSize::A3));
                pdfWriter.setResolution(300); // Set the resolution as needed

                // Create a QPainter to draw on the PDF
                QPainter painter(&pdfWriter);
                painter.setRenderHint(QPainter::Antialiasing);

                // Open a new page for the PDF
                painter.begin(&pdfWriter);

                // Define fonts and styles as needed for your PDF
                QFont font;
                font.setPointSize(10); // Adjust font size as needed
                painter.setFont(font);

                // Fetch the data from your database (replace this with your actual query)
                QSqlQueryModel model;
                model.setQuery("SELECT * FROM PRODUITS"); // Modify the SQL query accordingly

                // Define the initial vertical offset and spacing between rows
                int yOffset = 50; // Initial vertical offset
                int rowHeight = 70; // Vertical spacing between rows

                // Define column widths and titles
                 QVector<int> columnWidths = {400, 485, 410, 320, 390, 485, 400, 500, 400}; // Adjust widths as needed
                QStringList columnTitles = {"QUANTITE", "EMPLACEMENT", "DATE_ENTREE", "DATE_ESPIRATION"};

                // Draw table header
                for (int col = 0; col < columnTitles.size(); ++col) {
                    painter.drawText(100 + std::accumulate(columnWidths.begin(), columnWidths.begin() + col, 0), yOffset, columnTitles[col]);
                }
                yOffset += rowHeight; // Move to the next row

                // Iterate through the data and write it to the PDF table
                for (int row = 0; row < model.rowCount(); ++row) {
                    for (int col = 0; col < columnWidths.size(); ++col) {
                        QString cellData = model.record(row).value(col).toString();
                        painter.drawText(100 + std::accumulate(columnWidths.begin(), columnWidths.begin() + col, 0), yOffset, cellData);
                    }
                    yOffset += rowHeight; // Move to the next row
                }

                // Finish drawing to the PDF
                painter.end();

                // Show a success message
                QMessageBox::information(this, "Export Successful", "Data exported to PDF successfully.");
}




void MainWindow::on_enregistrerButton_8_clicked()
{
    produits p;
    QSqlQueryModel *model = nullptr;

    if (ui->radioButton_4->isChecked()) {
        model = p.tri_par_EMPLACEMENT_croissant();
    } else if (ui->radioButton_4->isChecked()) {
        model = p.tri_par_EMPLACEMENT_decroissant();
    } else if (ui->radioButton_7->isChecked()) {
        model = p.tri_par_id_croissant();
    } else if (ui->triparemplacement->isChecked()) {
        model = p.tri_par_id_decroissant();
    } else if (ui->triparquantite->isChecked()) {
        model = p.tri_par_QUANTITE_decroissant();
    } else if (ui->radioButton_7->isChecked()) {
        model = p.tri_par_QUANTITE_croissant();
    }

    ui->tableView->setModel(model);
}

void MainWindow::sendTotalQuantityToArduino()
{
    // Query to get the total quantity
    QSqlQuery query;
    query.prepare("SELECT SUM(QUANTITE) FROM PRODUITS");

    if (!query.exec()) {
        qDebug() << "Error fetching total quantity: " << query.lastError().text();
        return;
    }

    // Get the result
    if (query.next()) {
        int totalQuantity = query.value(0).toInt();
        qDebug() << "Total Quantity: " << totalQuantity;

        // Send the total quantity to Arduino
        sendToArduino(totalQuantity);
    }
}

void MainWindow::sendToArduino(int totalQuantity)
{
    // Assuming you have an open serial port connection (configure it before calling this function)
    if (serial.isOpen()) {
        QString quantityString = QString::number(totalQuantity) + "\n"; // Add a newline to indicate end of transmission
        serial.write(quantityString.toUtf8());
    } else {
        qDebug() << "Serial port not open.";
    }
}


void MainWindow::on_pushButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select Image"), "", tr("Images (*.png *.jpg *.jpeg *.bmp)"));
    QString id = ui->b->text();
        if (filePath.isEmpty()) {
            QMessageBox::warning(this, tr("No Image Selected"), tr("Please select an image to insert."));
            return;
        }

        // Open the image file
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("File Error"), tr("Could not open the selected image."));
            return;
        }

        // Read the file data into a QByteArray
        QByteArray imageData = file.readAll();
        file.close();

        // Prepare the SQL query to insert the image
        QSqlQuery query;
        query.prepare("UPDATE PATIENT SET IMAGE=:imageData WHERE ID_PROD=:ID");
        query.bindValue(":imageData", imageData);
        query.bindValue(":ID_PROD", id);

        // Execute the query and check for errors
        if (!query.exec()) {
            QMessageBox::critical(this, tr("Database Error"), tr("Failed to insert image into database: ") + query.lastError().text());
        } else {
            QMessageBox::information(this, tr("Success"), tr("Image inserted successfully into the database."));
        }
}


/*void MainWindow::on_enregistrerButton_10_clicked()
{
    // Set the current tab to the second tab (index 2)


        // Get the id from the input field (assuming it's a text input)
        QString id = ui->b->text();

        // Prepare the SQL query to fetch the image data
        QSqlQuery query;
        query.prepare("SELECT IMAGE FROM PRODUITS WHERE ID_PROD = :id");
        query.bindValue(":id", id);

        // Execute the query
        if (!query.exec()) {
            QMessageBox::critical(this, tr("Database Error"), tr("Failed to fetch image: ") + query.lastError().text());
            return;
        }

        // Check if there is any result
        if (query.next()) {
            // Retrieve the image data (as QByteArray)
            QByteArray imageData = query.value(0).toByteArray();

            // Check if the image data is valid
            if (!imageData.isEmpty()) {
                // Create a QPixmap from the QByteArray
                QPixmap pixmap;
                if (pixmap.loadFromData(imageData)) {
                    // Set the pixmap on the label
                    ui->f_2->setPixmap(pixmap);  // Error
                    ui->f_2->setScaledContents(true); // Error
                     // To make the image fit inside the label
                } else {
                    QMessageBox::warning(this, tr("Image Error"), tr("Failed to load the image."));
                }
            } else {
                QMessageBox::warning(this, tr("No Image"), tr("No image found for the given ID."));
            }
        } else {
            QMessageBox::warning(this, tr("Not Found"), tr("No record found for the provided ID."));
        }
}
*/
