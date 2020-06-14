#include <iostream>
#include <string>

#include "ivy.h"

int main(int argc, const char *argv[])
{
  HTMLParser *htmlParser = generateAntlr4Parser<HTMLLexer, HTMLParser>(argv[1]);
  HTMLParser::HtmlDocumentContext *html = htmlParser->htmlDocument();

  CSSParser *cssParser = generateAntlr4Parser<CSSLexer, CSSParser>(argv[2]);
  CSSParser::StylesheetContext *css = cssParser->stylesheet();

  HTMLVisitor htmlVisitor;
  htmlVisitor.parseHtml(html);

  CSSVisitor cssVisitor;
  cssVisitor.parseCss(css);
  return 0;
}
