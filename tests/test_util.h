#include <experimental/filesystem>

#include "ivy.h"

#pragma once

namespace fs = std::experimental::filesystem;
fs::path testResource(std::string file);

std::shared_ptr<ElementNode> parseHtml(std::string file);
std::shared_ptr<Stylesheet> parseCSS(std::string file);
