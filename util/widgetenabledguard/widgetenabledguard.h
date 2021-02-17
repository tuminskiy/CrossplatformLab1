#pragma once

#include <vector>

#include <QWidget>

namespace util {

class WidgetEnabledGuard
{
  std::vector<QWidget*> widgets_;

public:
  WidgetEnabledGuard(std::initializer_list<QWidget*> ilist);
  ~WidgetEnabledGuard();
};

} // namespace util
