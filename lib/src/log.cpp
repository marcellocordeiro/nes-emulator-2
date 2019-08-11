#include "log.h"

namespace lib {
void log::set_stream(std::ostream& ref)
{
  this->stream = &ref;
}

void log::set_level(int value)
{
  this->level = value;
}

std::ostream& log::get_stream()
{
  return *this->stream;
}

int log::get_level() const
{
  return this->level;
}

log& log::get()
{
  static log instance;
  return instance;
}
}  // namespace lib
