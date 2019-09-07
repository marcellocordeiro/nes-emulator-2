#pragma once

#include <fstream>

#include "utility/file_manager.h"

namespace nes {
class emulator;

class debugger {
public:
  debugger(emulator&);

  void cpu_log();

private:
  emulator& emu;

  std::ofstream nestest_log{util::fmngr.get_app_path() / "nestest_out.log"};
};
}  // namespace nes
