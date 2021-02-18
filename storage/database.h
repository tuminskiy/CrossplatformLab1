#pragma once

#include <QFile>
#include <QDataStream>
#include <QDebug>

#include <vector>

namespace storage {

template <class Value>
class Database
{
  const QString filename_;

public:
  using value_type = Value;
  using const_reference = const value_type&;

  explicit Database(const QString& filename) : filename_(filename) { }

  bool save(const_reference value)
  {
    QFile file(filename_);

    if (!open_file(file, QIODevice::ReadWrite))
      return false;

    QDataStream ds(&file);
    ds.skipRawData(file.size());

    ds << value;

    return true;
  }

  std::vector<value_type> load() const
  {
    QFile file(filename_);

    if (!open_file(file, QIODevice::ReadOnly))
      return {};

    QDataStream ds(&file);

    std::vector<value_type> result;
    result.reserve(file.size() / sizeof(value_type));

    while (!ds.atEnd()) {
      value_type value;
      ds >> value;
      result.push_back(std::move(value));
    }

    return result;
  }

  void clear()
  {
    QFile file(filename_);
    file.resize(0);
  }

private:

  bool open_file(QFile& file, QIODevice::OpenMode mode) const
  {
    if (file.open(mode))
      return true;

    qDebug() << "Failed create/open file: " << filename_;
    return false;
  }
};

} // namespace storage

