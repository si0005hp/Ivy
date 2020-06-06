#include <iostream>
#include <string>

#include "antlr4-runtime.h"
#include "HTMLLexer.h"
#include "HTMLParser.h"

#include "ivy.h"

using namespace antlr4;

template <typename L, typename P>
P *generateParser(std::string fpath)
{
  std::ifstream stream;
  stream.open(fpath);
  ANTLRInputStream *input = new ANTLRInputStream(stream);
  L *lexer = new L(input);
  CommonTokenStream *tokens = new CommonTokenStream(lexer);
  P *parser = new P(tokens);
  return parser;
}

int main(int argc, const char *argv[])
{
  HTMLParser *htmlParser = generateParser<HTMLLexer, HTMLParser>(argv[1]);
  HTMLParser::HtmlDocumentContext *html = htmlParser->htmlDocument();
  std::cout << html->toStringTree(htmlParser) << std::endl;

  HTMLVisitor v;
  v.parseHtml(html);

  return 0;
}
