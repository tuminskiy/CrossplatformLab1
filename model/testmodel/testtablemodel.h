#pragma once

#include <QAbstractTableModel>
#include <QString>

#include <list>

#include "info.h"

namespace model {

class TestTableModel : public QAbstractTableModel
{
  // QLinkedList is deprecated
  std::list<Info> data_;

  using container = decltype(data_);
  using iterator = typename container::iterator;
  using const_iterator = typename container::const_iterator;

public:
  using value_type = typename container::value_type;
  using const_reference = typename container::const_reference;

  explicit TestTableModel(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = {}) const override;
  int columnCount(const QModelIndex& parent = {}) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

  Qt::ItemFlags flags(const QModelIndex& index) const override;

  void push_front(const_reference value);
  void push_back(const_reference value);

  void pop_front();
  void pop_back();

  void insert(int pos, const_reference value);
  void insert(int pos, std::initializer_list<value_type> ilist);
  template <class Iterator>
  void insert(int pos, Iterator first, Iterator last)
  {
    if (!is_valid_pos(pos))
      return ;

    beginInsertRows({}, pos, data_.size() + std::distance(first, last) - 1);
    data_.insert(it_by_pos(pos), first, last);
    endInsertRows();
  }

  void erase(int pos);
  void erase(int first, int last);

  void clear();

  const_iterator begin() const;
  const_iterator end() const;

private:
  QVariant get_value(const QModelIndex& index) const;

  bool is_valid_pos(int pos) const;

  iterator it_by_pos(int pos);
  const_iterator it_by_pos(int pos) const;

};

} // namespace model
