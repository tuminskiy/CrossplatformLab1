#include "testtablemodel.h"

#include <QColor>

namespace model {

TestTableModel::TestTableModel(QObject* parent)
  : QAbstractTableModel(parent)
  , data_()
{ }


int TestTableModel::rowCount(const QModelIndex& parent) const
{
  return static_cast<int>(data_.size());
}

int TestTableModel::columnCount(const QModelIndex& parent) const
{
  return 2;
}


QVariant TestTableModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return {};

  switch (role) {
  case Qt::EditRole:
  case Qt::DisplayRole: return get_value(index);
  default: return {};
  }
}

QVariant TestTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
    return {};

  QVariant result;

  switch (section) {
  case 0: result = "Id"; break;
  case 1: result = "Name"; break;
  }

  return result;
}

bool TestTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (!index.isValid() || role != Qt::EditRole)
    return false;

  const auto info = qvariant_cast<Info>(value);

  auto it = it_by_pos(index.row());

  switch (index.column()) {
  case 0: it->id = value.toUInt(); break;
  case 1: it->name = value.toString(); break;
  }

  emit dataChanged(index, index);

  return true;
}


Qt::ItemFlags TestTableModel::flags(const QModelIndex& index) const
{
  return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}


void TestTableModel::push_front(const_reference value)
{
  beginInsertRows({}, 0, 0);
  data_.push_front(value);
  endInsertRows();
}

void TestTableModel::push_back(const_reference value)
{
  const auto row = data_.size();

  beginInsertRows({}, row, row);
  insertRow(row);
  data_.push_back(value);
  endInsertRows();
}


void TestTableModel::pop_front()
{
  beginRemoveRows({}, 0, 0);
  data_.pop_front();
  endRemoveRows();
}

void TestTableModel::pop_back()
{
  const auto row = data_.size();

  beginRemoveRows({}, row, row);
  data_.pop_back();
  endRemoveRows();
}


void TestTableModel::insert(int pos, const_reference value)
{
  if (!is_valid_pos(pos))
    return;

  beginInsertRows({}, pos, pos);
  data_.insert(it_by_pos(pos), value);
  endInsertRows();
}

void TestTableModel::insert(int pos, std::initializer_list<value_type> ilist)
{
  if (!is_valid_pos(pos))
    return;

  beginInsertRows({}, pos, pos + ilist.size());
  data_.insert(it_by_pos(pos), ilist);
  endRemoveRows();
}


void TestTableModel::erase(int pos)
{
  if (!is_valid_pos(pos))
    return;

  beginRemoveRows({}, pos, pos);
  data_.erase(it_by_pos(pos));
  endRemoveRows();
}

void TestTableModel::erase(int first, int last)
{
  if ((first >= last) || !is_valid_pos(first) || !is_valid_pos(last))
    return;

  const auto first_it = it_by_pos(first);
  const auto last_it = [&]() {
    auto it = first_it;
    std::advance(it, last - first);
    return it;
  }();

  beginRemoveRows({}, first, last);
  data_.erase(first_it, last_it);
  endRemoveRows();
}


void TestTableModel::clear()
{
  beginRemoveRows({}, 0, data_.size());
  data_.clear();
  endRemoveRows();
}


auto TestTableModel::begin() const -> const_iterator { return data_.begin(); }

auto TestTableModel::end() const -> const_iterator { return data_.end(); }


QVariant TestTableModel::get_value(const QModelIndex& index) const
{
  const auto it = it_by_pos(index.row());

  QVariant result;

  switch (index.column()) {
  case 0: result = it->id; break;
  case 1: result = it->name; break;
  }

  return result;
}

bool TestTableModel::is_valid_pos(int pos) const
{
  return pos >= 0 && pos < data_.size();
}

auto TestTableModel::it_by_pos(int pos) -> iterator
{
  auto it = data_.begin();
  std::advance(it, pos);
  return it;
}

auto TestTableModel::it_by_pos(int pos) const -> const_iterator
{
  auto it = data_.begin();
  std::advance(it, pos);
  return it;
}

} // namespace model
