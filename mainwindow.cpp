#include "mainwindow.h"
#include "modifierf.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQueryModel>
#include <QRegularExpression>
#include <QPdfWriter>
#include <QTextCursor>
#include <QDesktopServices>
#include <QTextTable>
#include <QLineEdit>
#include <QFileDialog>
#include "fournisseur.h"
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QToolTip>
#include <QDate>
#include <QMessageBox>
#include "mainwindow.h"
#include "calender.h"
#include <QCalendar>
#include <QStandardPaths>
#include <QPrinter>
#include <QTextDocument>
#include <QInputDialog>






// Constructor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->locationComboBox->addItems({
        "Tunis", "Sfax", "Sousse", "Gabès", "Bizerte", "Ariana", "Kairouan",
        "Gafsa", "La Marsa", "Monastir", "Nabeul", "Ben Arous", "Hammamet",
        "Mahdia", "Zarzis", "Douz", "Tozeur", "Kebili", "Beja", "Jendouba",
        "Siliana", "Kasserine", "Le Kef", "Medenine", "Tataouine"
    });

    // Connect row selection to populate fields
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &MainWindow::onRowSelected);

    // Connect double-click on a row to populate the fields
    connect(ui->tableView, &QTableView::doubleClicked,
            this, &MainWindow::onRowSelected);

    loadFournisseurData();

    connect(ui->pdfIN, &QPushButton::clicked, this, &MainWindow::on_pdfIN_clicked);
    connect(ui->sortCheck, &QCheckBox::clicked, this, &MainWindow::on_sortCheck_clicked);
    connect(ui->statisticsButton, &QPushButton::clicked, this, &MainWindow::showStatistics);

    logHistory("Application started");

    qDebug() << "Available database drivers: " << QSqlDatabase::drivers();
    ui->tableView->setModel(F.afficher());
}

// Destructor
MainWindow::~MainWindow()
{
    logHistory("Application closed");
    delete ui;
}

// Log history function
void MainWindow::logHistory(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    ui->historyTextBrowser->append("[" + timestamp + "] " + message);
}

// Row selection
void MainWindow::onRowSelected(const QModelIndex &index)
{
    if (index.isValid()) {
        int row = index.row();
        ui->idIN->setText(ui->tableView->model()->index(row, 0).data().toString());
        ui->nomIN->setText(ui->tableView->model()->index(row, 1).data().toString());
        ui->prenomIN->setText(ui->tableView->model()->index(row, 2).data().toString());
        ui->locationComboBox->setCurrentText(ui->tableView->model()->index(row, 3).data().toString());
        ui->telIN->setText(ui->tableView->model()->index(row, 4).data().toString());
        ui->emailIN->setText(ui->tableView->model()->index(row, 5).data().toString());

        logHistory("Row selected: ID=" + ui->idIN->text());
    }
}

// Validation function
QString MainWindow::controlSaisie()
{
    QRegularExpression idRegex(R"(^\d{6}$)");
    QRegularExpression emailRegex(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
    QRegularExpression nameRegex(R"(^[A-Za-zÀ-ÿ\s-]{1,50}$)");
    QRegularExpression phoneRegex(R"(^\d{8,15}$)");

    if (!idRegex.match(ui->idIN->text()).hasMatch())
        return "ID must be a valid number digits.";
    if (ui->idIN->text().isEmpty())
        return "ID cannot be empty.";
    if (!nameRegex.match(ui->nomIN->text()).hasMatch())
        return "Nom must only contain letters and be 1-50 characters.";
    if (!nameRegex.match(ui->prenomIN->text()).hasMatch())
        return "Prenom must only contain letters and be 1-50 characters.";
    if (!phoneRegex.match(ui->telIN->text()).hasMatch())
        return "Téléphone must be a valid number (8-15 digits).";
    if (!emailRegex.match(ui->emailIN->text()).hasMatch())
        return "Email format is invalid.";
    if (ui->locationComboBox->currentText().isEmpty())
        return "Location cannot be empty.";

    return "";  // No errors found
}

// Add record
void MainWindow::on_ajoutBTN_clicked()
{
    QString id = ui->idIN->text();
    QString nom = ui->nomIN->text();
    QString prenom = ui->prenomIN->text();
    QString adresse = ui->locationComboBox->currentText();
    QString tel = ui->telIN->text();
    QString email = ui->emailIN->text();
    fournisseur F(id, nom, prenom, adresse, tel, email);

    QString validationMessage = controlSaisie();
    if (validationMessage.isEmpty()) {
        if (F.ajouter()) {
            QMessageBox::information(this, QObject::tr("OK"),
                                     QObject::tr("Ajout effectué\n"), QMessageBox::Ok);
            loadFournisseurData();
            logHistory("Added fournisseur: ID=" + id + ", Name=" + nom + ", Prenom=" + prenom);
        } else {
            QMessageBox::critical(this, QObject::tr("Not OK"),
                                  QObject::tr("Ajout non effectué."), QMessageBox::Cancel);
            logHistory("Failed to add fournisseur: ID=" + id);
        }
    } else {
        QMessageBox::warning(this, "Input Error", validationMessage);
    }
}

// Modify record
void MainWindow::on_modifierBTN_clicked()
{
    QString id = ui->idIN->text();
    QString nom = ui->nomIN->text();
    QString prenom = ui->prenomIN->text();
    QString location = ui->locationComboBox->currentText();
    QString tel = ui->telIN->text();
    QString email = ui->emailIN->text();
    fournisseur F(id, nom, prenom, location, tel, email);

    QString validationMessage = controlSaisie();
    if (validationMessage.isEmpty()) {
        if (F.modifier(id)) {
            QMessageBox::information(this, QObject::tr("OK"),
                                     QObject::tr("Modification réussie."), QMessageBox::Ok);
            loadFournisseurData();
            logHistory("Modified fournisseur: ID=" + id);
        } else {
            QMessageBox::critical(this, QObject::tr("Update Failed"),
                                  QObject::tr("Modification échouée."), QMessageBox::Cancel);
            logHistory("Failed to modify fournisseur: ID=" + id);
        }
    } else {
        QMessageBox::warning(this, "Input Error", validationMessage);
    }
}

// Delete record
void MainWindow::on_supprimerBTN_clicked()
{
    QModelIndex selectedIndex = ui->tableView->currentIndex();

    if (!selectedIndex.isValid()) {
        QMessageBox::warning(this, "Selection Error", "Please select a row to delete.");
        return;
    }

    int row = selectedIndex.row();
    QString id = ui->tableView->model()->index(row, 0).data().toString();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Deletion", "Are you sure you want to delete this record?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        fournisseur F;
        if (F.supprimer(id)) {
            QMessageBox::information(this, "Success", "Record deleted successfully.");
            loadFournisseurData();
            logHistory("Deleted fournisseur: ID=" + id);
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete the record.");
            logHistory("Failed to delete fournisseur: ID=" + id);
        }
    }
}

// Load data
void MainWindow::loadFournisseurData()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT ID_F, NOM_F, PRENOM_F, ADRESSE_F, TEL_F, EMAIL_F FROM fournisseur");

    if (model->lastError().isValid()) {
        qDebug() << "Error loading data from database: " << model->lastError().text();
        return;
    }

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    logHistory("Loaded fournisseur data");
}


void MainWindow::on_pdfIN_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "Fichiers PDF (*.pdf)");
    if (!fileName.isEmpty()) {
        QSqlQueryModel* model = F.afficher();
        if (F.exporterPDF(fileName, model)) {
            QMessageBox::information(this, "Exportation PDF", "Les données ont été exportées en PDF avec succès !");
        } else {
            QMessageBox::warning(this, "Exportation PDF", "Échec de l'exportation en PDF.");
        }
    }
}



void MainWindow::on_rechercheIN_clicked()
{
    QString id = ui->IDr->text();  // Assuming you have an input field for the ID
    if (id.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid ID.");
        return;
    }

    QSqlQueryModel* model = new QSqlQueryModel(this);
    QSqlQuery query;
    query.prepare("SELECT ID_F, NOM_F, PRENOM_F, ADRESSE_F, TEL_F, EMAIL_F FROM fournisseur WHERE ID_F = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        model->setQuery(query);
        if (model->rowCount() > 0) {
            ui->tableView->setModel(model); // Display the results in the table view
            ui->tableView->resizeColumnsToContents();
            logHistory("Searched fournisseur by ID: " + id);
        } else {
            QMessageBox::information(this, "No Results", "No fournisseur found with the given ID.");
            logHistory("No results found for ID: " + id);
        }
    } else {
        QMessageBox::critical(this, "Database Error", "Failed to execute search query.");
        qDebug() << "Database error: " << query.lastError().text();
    }
}


/*void MainWindow::on_calendrier_clicked()
{
    // Create a new instance of calendrier (your calendar widget)
    calendrier *calendarView = new calendrier(this);  // Assuming you have the class `calendrier`

    // Call the function to populate the calendar with data
    calendarView->populateCalendarWithData();  // Populate the calendar with data

    // Show the calendar view window
    calendarView->show();
}*/

void MainWindow::showStatistics()
{
    // Remove any existing widgets from the stat layout
        QLayout *layout = ui->stat->layout();
        if (layout != nullptr) {
            QLayoutItem *item;
            while ((item = layout->takeAt(0)) != nullptr) {
                delete item->widget();  // Delete the widget if it exists
                delete item;  // Delete the layout item
            }
        }

        QMap<QString, int> stats = F.getCountByAdresse();
        QPieSeries *series = new QPieSeries();

        // Populate the series with data
        int total = 0;
        for (auto it = stats.begin(); it != stats.end(); ++it) {
            total += it.value();
        }

        for (auto it = stats.begin(); it != stats.end(); ++it) {
            QPieSlice *slice = series->append(it.key() + " (" + QString::number(it.value()) + ")", it.value());
            slice->setLabelVisible(true);

            // Explode slices with values > 25% of total
            if (it.value() > 0.25 * total) {
                slice->setExploded(true);
                slice->setExplodeDistanceFactor(0.1);  // Adjust explode distance
                slice->setLabelColor(Qt::red);  // Highlight large values with red labels
                slice->setBrush(Qt::darkRed);  // Highlight slice color
            }
        }

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Nombre des fournisseurs par gouvernement:");
        chart->setTitleFont(QFont("Arial", 14, QFont::Bold));  // Bigger, bold title font
        chart->legend()->setAlignment(Qt::AlignRight);
        chart->legend()->setFont(QFont("Arial", 10));  // Larger legend font

        // Create a chart view and set it up
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        // Add the new chart view to the layout
        layout->addWidget(chartView);

}



void MainWindow::on_statisticsButton_clicked()
{
    showStatistics();
}


void MainWindow::on_sortCheck_clicked()
{
    QSqlQueryModel* model = F.trierParNom();
        ui->tableView->setModel(model);
}




void MainWindow::on_calendrier_clicked()
{
    QCalendarWidget *calendar = new QCalendarWidget(this);
    calendar->setWindowFlags(Qt::Window);
    calendar->resize(400, 300);
    calendar->show();

    // Connect the calendar's clicked signal to handle date selection
    connect(calendar, &QCalendarWidget::clicked, this, [=](const QDate &date){
        qDebug() << "Selected date:" << date.toString();
        // You can do something with the selected date here
    });
}

/*void MainWindow::loadLogFileContent()
{

    QString logFilePath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/logfile.log";

    QFile logFile(logFilePath);

    if (logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&logFile);
        QString content = in.readAll();
        ui->textBrowser->setText(content);
        logFile.close();
    } else {

        ui->textBrowser->setText("Error: Unable to open the log file.");
        QMessageBox::warning(this, "File Error", "Failed to open the log file. Please check its location and permissions.");
    }
}
*/


/*void MainWindow::logEvent(const QString &event)
{

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");


    QString logFilePath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/logfile.log";


    QFile logFile(logFilePath);
    if (!logFile.exists()) {
        if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {

            QMessageBox::critical(nullptr, QObject::tr("Error"),
                                  QObject::tr("Failed to create log file."),
                                  QMessageBox::Ok);
            return;
        }
        logFile.close();
    }


    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
        // Create a text stream and write the log information
        QTextStream out(&logFile);
        out << timestamp << " - " << event << "\n";


        logFile.close();
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Error"),
                              QObject::tr("Failed to open log file for writing."),
                              QMessageBox::Ok);
    }
}
*/

void MainWindow::on_rechercheIN_2_clicked()
{
    QString name = QInputDialog::getText(this, "Search Fournisseur", "Enter the name:");
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid name.");
        return;
    }

    QSqlQueryModel* model = new QSqlQueryModel(this);
    QSqlQuery query;
    query.prepare("SELECT ID_F, NOM_F, PRENOM_F, ADRESSE_F, TEL_F, EMAIL_F FROM fournisseur WHERE NOM_F LIKE :name");
    query.bindValue(":name", "%" + name + "%");

    if (query.exec()) {
        model->setQuery(query);
        if (model->rowCount() > 0) {
            ui->tableView->setModel(model); // Display the results in the table view
            ui->tableView->resizeColumnsToContents();
            logHistory("Searched fournisseur by name: " + name);
        } else {
            QMessageBox::information(this, "No Results", "No fournisseur found with the given name.");
            logHistory("No results found for name: " + name);
        }
    } else {
        QMessageBox::critical(this, "Database Error", "Failed to execute search query.");
        qDebug() << "Database error: " << query.lastError().text();
    }
}



void MainWindow::on_rechercheIN_3_clicked()
{
    // Populate the combo box with unique addresses from the database
    QSqlQuery query("SELECT DISTINCT ADRESSE_F FROM fournisseur");
    QStringList addresses;
    while (query.next()) {
        addresses << query.value(0).toString();
    }

    // Check if there are any addresses to display
    if (addresses.isEmpty()) {
        QMessageBox::information(this, "No Addresses", "No addresses found in the database.");
        return;
    }

    // Create a combo box dialog for address selection
    bool ok;
    QString selectedAddress = QInputDialog::getItem(this, "Select Address",
                                                    "Choose an address:", addresses, 0, false, &ok);

    // If the user confirms the selection
    if (ok && !selectedAddress.isEmpty()) {
        // Query to find fournisseurs by the selected address
        QSqlQueryModel *model = new QSqlQueryModel(this);
        QSqlQuery searchQuery;
        searchQuery.prepare("SELECT ID_F, NOM_F, PRENOM_F, ADRESSE_F, TEL_F, EMAIL_F FROM fournisseur WHERE ADRESSE_F = :address");
        searchQuery.bindValue(":address", selectedAddress);

        if (searchQuery.exec()) {
            model->setQuery(searchQuery);

            if (model->rowCount() > 0) {
                // Display the results in the table view
                ui->tableView->setModel(model);
                ui->tableView->resizeColumnsToContents();
                logHistory("Displayed fournisseurs for address: " + selectedAddress);
            } else {
                QMessageBox::information(this, "No Results", "No fournisseur found for the selected address.");
            }
        } else {
            // Query execution error
            qDebug() << "Database error: " << searchQuery.lastError().text();
            QMessageBox::critical(this, "Database Error", "Failed to execute search query.");
        }
    }
}




void MainWindow::on_refresh_clicked()
{
    QSqlQueryModel* model = new QSqlQueryModel(this);
        QSqlQuery query;
        query.prepare("SELECT ID_F, NOM_F, PRENOM_F, ADRESSE_F, TEL_F, EMAIL_F FROM fournisseur");

        if (query.exec()) {
            model->setQuery(query);
            ui->tableView->setModel(model); // Display all fournisseurs in the table view
            ui->tableView->resizeColumnsToContents();
            logHistory("Refreshed the fournisseurs table.");
        } else {
            QMessageBox::critical(this, "Database Error", "Failed to load fournisseurs.");
            qDebug() << "Database error: " << query.lastError().text();
        }
}

