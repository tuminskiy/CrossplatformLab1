#pragma once

#include "ui_infodialog.h"

#include "info.h"

class InfoDialog : public QDialog
{
  Q_OBJECT

  Ui::InfoDialog ui_;

public:
  explicit InfoDialog(Info info = {}, QWidget *parent = nullptr);

signals:
  void oked(const Info& info);

private slots:
  void ok_click();

private:
  bool is_valid_input() const;
};

