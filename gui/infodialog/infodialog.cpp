#include "infodialog.h"

#include <QMessageBox>

InfoDialog::InfoDialog(Info info, QWidget *parent)
  : QDialog(parent)
{
  ui_.setupUi(this);

  ui_.leId->setText(QString::number(info.id));
  ui_.leName->setText(info.name);

  connect(ui_.bOk, &QPushButton::clicked,
          this, &InfoDialog::ok_click);
  connect(ui_.bCancel, &QPushButton::clicked,
          this, &InfoDialog::done);
}

void InfoDialog::ok_click()
{
  if (!is_valid_input()) {
    QMessageBox::critical(this, "Critical", "All fields must be fill");
    return;
  }

  Info result;
  result.id = ui_.leId->text().toUInt();
  result.name = ui_.leName->text();

  emit oked(result);
  done(0);
}

bool InfoDialog::is_valid_input() const
{
  return !(ui_.leId->text().isEmpty() || ui_.leName->text().isEmpty());
}
