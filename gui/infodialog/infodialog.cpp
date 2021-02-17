#include "infodialog.h"

InfoDialog::InfoDialog(Info info, QWidget *parent)
  : QDialog(parent)
{
  ui.setupUi(this);

  ui.leId->setText(QString::number(info.id));
  ui.leName->setText(info.name);

  connect(ui.bOk, &QPushButton::clicked,
          this, &InfoDialog::ok_click);
  connect(ui.bCancel, &QPushButton::clicked,
          this, &InfoDialog::done);
}

void InfoDialog::ok_click()
{
  Info result;
  result.id = ui.leId->text().toUInt();
  result.name = ui.leName->text();

  emit oked(result);
  done(0);
}
