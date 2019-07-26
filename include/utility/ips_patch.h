#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

#include "common.h"

namespace nes::util {
class ips_patch {
public:
  ips_patch(const std::filesystem::path&);

  std::vector<uint8_t> patch(const std::vector<uint8_t>&);

private:
  bool check();
  void build();
  bool read_record();

  struct record_entry {
    uint32_t             addr   = 0;
    uint16_t             length = 0;
    std::vector<uint8_t> data;
  };

  std::vector<record_entry> records;
  std::ifstream             ips_file;
  size_t                    min_size = 0;
};
}  // namespace nes::util
