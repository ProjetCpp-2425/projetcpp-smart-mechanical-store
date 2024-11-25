#include "modifierf.h"
#include "ui_modifierf.h"

modifierf::modifierf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modifierf)
{
    ui->setupUi(this);
}

modifierf::~modifierf()
{
    delete ui;
}
