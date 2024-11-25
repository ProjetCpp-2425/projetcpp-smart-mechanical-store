#ifndef MODIFIERF_H
#define MODIFIERF_H

#include <QDialog>

namespace Ui {
class modifierf;
}

class modifierf : public QDialog
{
    Q_OBJECT

public:
    explicit modifierf(QWidget *parent = nullptr);
    ~modifierf();

private:
    Ui::modifierf *ui;
};

#endif // MODIFIERF_H
