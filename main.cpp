#include <iostream>
#include <string>

#include "antlr4-runtime.h"
#include "HTMLLexer.h"
#include "HTMLParser.h"
#include "HTMLParserBaseVisitor.h"
#include "HTMLParserBaseListener.h"

using namespace antlr4;

int main(int argc, const char *argv[])
{
  std::ifstream stream;
  stream.open(argv[1]);
  ANTLRInputStream input(stream);
  HTMLLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  HTMLParser parser(&tokens);

  tree::ParseTree *tree = parser.htmlDocument();
  std::cout << tree->toStringTree(&parser) << std::endl;
  return 0;
}
