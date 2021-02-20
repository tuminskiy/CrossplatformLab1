#pragma once

#include <QMainWindow>
#include "ui_mainwindow.h"

#include <memory>

#include "model/testmodel/testtablemodel.h"
#include "info.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

  Ui::MainWindow ui_;
  model::TestTableModel model_;
  QModelIndexList selected_;

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

signals:
  void selected_changed();

private slots:
  void change_visability();

  void add_click();
  void delete_click();

  void select_rows(const QItemSelection& selected, const QItemSelection& deselected);

  void save_click();
  void load_click();
};
