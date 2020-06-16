#include <experimental/filesystem>
#include <cstdlib>

#pragma once

namespace fs = std::experimental::filesystem;

fs::path testResource(std::string file);
