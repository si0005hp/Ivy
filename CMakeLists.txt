cmake_minimum_required(VERSION 3.4)
project(main)

set(CMAKE_CXX_FLAGS "-g")

find_package(Java COMPONENTS Runtime REQUIRED)

set(antlr4-lib-cpp-dir ${PROJECT_SOURCE_DIR}/antlr4/runtime/Cpp)
add_subdirectory(${antlr4-lib-cpp-dir} build)


set(antlr4-html-GENERATED_SRC
  ${PROJECT_SOURCE_DIR}/generated/HTMLLexer.cpp
  ${PROJECT_SOURCE_DIR}/generated/HTMLParserBaseListener.cpp
  ${PROJECT_SOURCE_DIR}/generated/HTMLParserBaseVisitor.cpp
  ${PROJECT_SOURCE_DIR}/generated/HTMLParser.cpp
  ${PROJECT_SOURCE_DIR}/generated/HTMLParserListener.cpp
  ${PROJECT_SOURCE_DIR}/generated/HTMLParserVisitor.cpp
)
set(antlr4-css-GENERATED_SRC
  ${PROJECT_SOURCE_DIR}/generated/CSSLexer.cpp
  ${PROJECT_SOURCE_DIR}/generated/CSSParserBaseListener.cpp
  ${PROJECT_SOURCE_DIR}/generated/CSSParserBaseVisitor.cpp
  ${PROJECT_SOURCE_DIR}/generated/CSSParser.cpp
  ${PROJECT_SOURCE_DIR}/generated/CSSParserListener.cpp
  ${PROJECT_SOURCE_DIR}/generated/CSSParserVisitor.cpp
)

add_custom_target(GenerateParser DEPENDS ${antlr4-html-GENERATED_SRC} ${antlr4-css-GENERATED_SRC})
add_custom_command(OUTPUT ${antlr4-html-GENERATED_SRC} ${antlr4-css-GENERATED_SRC}
  COMMAND 
  ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/generated/
  COMMAND
  ${Java_JAVA_EXECUTABLE} -jar ${ANTLR_JAR_LOCATION} -Werror -Dlanguage=Cpp -listener -visitor -o ${PROJECT_SOURCE_DIR}/generated/ 
    ${PROJECT_SOURCE_DIR}/HTMLLexer.g4 ${PROJECT_SOURCE_DIR}/HTMLParser.g4
    ${PROJECT_SOURCE_DIR}/CSSLexer.g4 ${PROJECT_SOURCE_DIR}/CSSParser.g4

  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"

  DEPENDS ${PROJECT_SOURCE_DIR}/HTMLLexer.g4 ${PROJECT_SOURCE_DIR}/HTMLParser.g4
          ${PROJECT_SOURCE_DIR}/CSSLexer.g4 ${PROJECT_SOURCE_DIR}/CSSParser.g4
)

include_directories(
  ${antlr4-lib-cpp-dir}/runtime/src
  ${antlr4-lib-cpp-dir}/runtime/src/misc
  ${antlr4-lib-cpp-dir}/runtime/src/atn
  ${antlr4-lib-cpp-dir}/runtime/src/dfa
  ${antlr4-lib-cpp-dir}/runtime/src/tree
  ${antlr4-lib-cpp-dir}/runtime/src/support
  ${PROJECT_SOURCE_DIR}/generated
)

set(all_SRC
  ${PROJECT_SOURCE_DIR}/main.cpp
  ${PROJECT_SOURCE_DIR}/html.cpp
  ${antlr4-html-GENERATED_SRC}
  ${antlr4-css-GENERATED_SRC}
)

add_executable(main ${all_SRC})
add_dependencies(main GenerateParser)
target_link_libraries(main antlr4_static)
