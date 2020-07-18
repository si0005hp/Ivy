#include <cstdlib>
#include <experimental/filesystem>

#include "test_util.h"

using namespace antlr4;

char *testResourcesDir = std::getenv("TEST_RESOURCES_DIR");
fs::path testResource(std::string file)
{
  fs::path dir = testResourcesDir ? testResourcesDir : "../tests/resources";
  return dir.append("/").append(file);
}

std::shared_ptr<ElementNode> parseHtml(std::string file)
{
  HTMLParser *htmlParser = generateAntlr4Parser<HTMLLexer, HTMLParser>(testResource(file));
  HTMLParser::HtmlDocumentContext *html = htmlParser->htmlDocument();

  return IvyHTMLParser().parseHtml(html);
}

std::shared_ptr<Stylesheet> parseCSS(std::string file)
{
  CSSParser *cssParser = generateAntlr4Parser<CSSLexer, CSSParser>(testResource(file));
  CSSParser::StylesheetContext *css = cssParser->stylesheet();

  return IvyCSSParser().parseCSS(css);
}

std::shared_ptr<StyledNode> buildStyledNode(std::string htmlFilePath, std::string cssFilePath)
{
  std::shared_ptr<Node> htmlNode = parseHtml(htmlFilePath);
  std::shared_ptr<Stylesheet> stylesheet = parseCSS(cssFilePath);

  return StyledNodeBuilder().buildStyledNode(parseHtml(htmlFilePath), parseCSS(cssFilePath));
}
