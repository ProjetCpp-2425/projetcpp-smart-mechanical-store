#ifndef CALENDRIER_H
#define CALENDRIER_H

#include <QWidget>
#include <QCalendarWidget>
#include <QMap>
#include <QDate>
#include <QSqlQuery>
#include <QMessageBox>
#include <QTextCharFormat>



namespace Ui {
class calendrier;
}

class calendrier : public QWidget
{
    Q_OBJECT

public:
    explicit calendrier(QWidget *parent = nullptr);
    ~calendrier();

private:
    Ui::calendrier *ui; // Pointer to the UI class
};

#endif // CALENDRIER_H
