#include "widgetenabledguard.h"

#include <QWidget>

namespace util {

WidgetEnabledGuard::WidgetEnabledGuard(std::initializer_list<QWidget*> ilist)
  : widgets_(ilist)
{
  for (auto widget : widgets_) {
    widget->setEnabled(false);
  }
}

WidgetEnabledGuard::~WidgetEnabledGuard()
{
  for (auto widget : widgets_) {
    widget->setEnabled(true);
  }
}

} // namespace util
