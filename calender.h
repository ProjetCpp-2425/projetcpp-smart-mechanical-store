#ifndef CALENDER_H
#define CALENDER_H

#include <QWidget>
#include <QCalendarWidget>
#include <QMap>
#include <QDate>
#include <QSqlQuery>
#include <QMessageBox>
#include <QTextCharFormat>

namespace Ui {
class Calender;
}

class Calender : public QWidget
{
    Q_OBJECT

public:
    explicit Calender(QWidget *parent = nullptr);
    ~Calender();

    void populateCalendarWithData(QCalendarWidget *calendar);

private slots:
    void onDateSelected(const QDate &date); // Slot for handling date selection

private:
    Ui::Calender *ui;
};

#endif // CALENDER_H
