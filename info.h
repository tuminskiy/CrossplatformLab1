#pragma once

#include <QString>
#include <QMetaType>

struct Info
{
  uint32_t id;
  QString name;
};

template <class Stream>
Stream& operator<< (Stream& os, const Info& info)
{
  return os << info.id << info.name;
}

template <class Stream>
Stream& operator>> (Stream& os, Info& info)
{
  return os >> info.id >> info.name;
}

Q_DECLARE_METATYPE(Info);
