#include "mainwindow.h"

#include "gui/infodialog/infodialog.h"
#include "util/widgetenabledguard/widgetenabledguard.h"
#include "util/functional/functional.h"
#include "util/indexadapter/indexadapter.h"
#include "storage/database.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  ui_.setupUi(this);
  ui_.tableView->setModel(&model_);

  change_visability();

  connect(ui_.bAdd, &QPushButton::clicked,
          this, &MainWindow::add_click);

  connect(ui_.bDelete, &QPushButton::clicked,
          this, &MainWindow::delete_click);

  connect(ui_.tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
          this, &MainWindow::select_rows);

  connect(this, &MainWindow::selected_changed,
          this, &MainWindow::change_visability);

  connect(ui_.aSave, &QAction::triggered,
          this, &MainWindow::save_click);

  connect(ui_.aLoad, &QAction::triggered,
          this, &MainWindow::load_click);
}

MainWindow::~MainWindow() { }


void MainWindow::change_visability()
{
  if (selected_.empty()) {
    ui_.bDelete->hide();
  } else {
    ui_.bDelete->show();
  }
}


void MainWindow::add_click()
{
  auto dialog = new InfoDialog();
  dialog->setAttribute(Qt::WA_DeleteOnClose);

  connect(dialog, &InfoDialog::oked,
          &model_, &model::TestTableModel::push_back);

  dialog->show();
}


void MainWindow::delete_click()
{
  util::WidgetEnabledGuard wguard = { ui_.bAdd, ui_.bDelete, ui_.menu};

  if (selected_.size() == 1) {
    model_.erase(selected_.front().row());
  } else {
    model_.erase(selected_.front().row(), selected_.back().row());
  }
}


void MainWindow::select_rows(const QItemSelection& /*selected*/, const QItemSelection& /*deselected*/)
{
  selected_ = ui_.tableView->selectionModel()->selectedIndexes();
  emit selected_changed();
}


void MainWindow::save_click()
{
  const auto path = QFileDialog::getSaveFileName(this, "Save", /*dir*/{}, "*.bin");

  if (path.isEmpty())
    return;

  util::WidgetEnabledGuard wguard = {ui_.bAdd, ui_.bDelete, ui_.menu};

  storage::Database<Info> db(path);
  db.clear();

  bool success = true;
  for (const auto& value : model_) {
    success &= db.save(value);
  }

  if (!success) {
    QMessageBox::critical(this, "Critical", "Failed save data");
  }
}

void MainWindow::load_click()
{
  const auto path = QFileDialog::getOpenFileName(this, "Load", /*dir*/{}, "*.bin");

  if (path.isEmpty())
    return;

  util::WidgetEnabledGuard wguard = {ui_.bAdd, ui_.bDelete, ui_.menu};

  storage::Database<Info> db(path);
  const auto data = db.load();

  if (data.empty()) {
    QMessageBox::critical(this, "Critical", "Incorrect file: " + path);
    return;
  }

  model_.clear();
  model_.insert(0, data.begin(), data.end());
}
