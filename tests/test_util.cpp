#include <cstdlib>
#include <experimental/filesystem>

#include "test_util.h"

char *testResourcesDir = std::getenv("TEST_RESOURCES_DIR");
fs::path testResource(std::string file)
{
  fs::path dir = testResourcesDir ? testResourcesDir : "../tests/resources";
  return dir.append("/").append(file);
}
