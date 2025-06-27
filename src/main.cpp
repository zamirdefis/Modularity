#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <string>
#include <vector>

#include "notifications.hpp"
#include "macros.hpp"

#define IN_CODE_ARGS
#define TEST_ARGS_PARSER

enum class ArgType : uint8_t {
  Invalid,
  Brief,
  Single,
  Value,
};

[[nodiscard]] static ArgType getArgType(std::string_view fullArg) {
  size_t realPartIndex;
  // printf("%d", (fullArg.size() < 2u || fullArg.front() != '-' 
  // || (realPartIndex = fullArg.find_first_not_of('-'), printf("rpi : %zu\n", realPartIndex), realPartIndex) == SIZE_MAX) ||
  // !std::isalpha(fullArg[realPartIndex])); ENDLN;
  if (fullArg.size() < 2u || fullArg.front() != '-' ||
    (realPartIndex = fullArg.find_first_not_of('-'), realPartIndex) == SIZE_MAX ||
    fullArg[realPartIndex] == '=') {
    return ArgType::Invalid;
  }
  const decltype(realPartIndex) equal = fullArg.find('=');
  if (fullArg[1u] == '-') {
    if (equal == SIZE_MAX) {
      return ArgType::Single;
    }
    return ArgType::Value;
  }
  if (equal != SIZE_MAX) {
    return ArgType::Invalid;
  }
  return ArgType::Brief;
}

[[nodiscard]] static std::vector<std::pair<std::string, std::string>> parseArg(const std::string& fullArg) {
  std::vector<std::pair<std::string, std::string>> result{};
  if (ArgType argInfo = getArgType(fullArg); argInfo == ArgType::Invalid) {
    co::error(static_cast<std::string>(NE_1) + " : \"" + fullArg + "\"");
    return result;
  } else if (argInfo == ArgType::Brief) {
    for (std::string::const_iterator citer = fullArg.cbegin() + 1u;
    citer != fullArg.cend(); ++citer) {
      result.push_back(std::make_pair(std::string(1, *citer), ""));
    }
    return result;
  } else if (argInfo == ArgType::Value) {
    const size_t equalIndex = fullArg.find('=');
    return {std::make_pair(fullArg.substr(2u, equalIndex - 2u),
    fullArg.substr(equalIndex + 1u, fullArg.size() - (equalIndex + 1u)))};
  }
  return {std::make_pair(fullArg.substr(2u, fullArg.size() - 2u), "")};
}

[[nodiscard, gnu::noinline]] extern auto
main(signed int argc, const char** argv) -> decltype(argc) {
 // std::string test("-zalupka");
  std::vector<std::string> argvs;
#ifdef IN_CODE_ARGS
  // argvs = {"--z=l", "--kkkk=1234", "--o=", "--help", "--H", "--", "---kkk", "-h", "", "-", "-=", "-a=z"};
  argvs = {
    // Корректные (должны работать)
    "-a",                     // краткий флаг
    "--long",                 // длинный флаг без значения
    "-abc",                   // комбинированные флаги → a, b, c
    "--key=value",            // стандартный ключ-значение
    "--key = value",          // пробелы вокруг =
    "--key= val ue ",         // пробелы в значении
    "--!@=!@#$%^",           // спецсимволы
    "--к ey=зна чение",       // Unicode с пробелами
    "--東京=Tok yo",          // иероглифы
    "--key=",                 // пустое значение
    "--key=val\nue",          // управляющие символы
    
    // Некорректные (должны вызывать ошибку)
    "-a=value",               // сокращённые аргументы не принимают значения
    "-abc=123",               // комбинированные флаги с значением
    "--=value",               // пустой ключ
    "-",                      // просто дефис
    "--",                     // двойной дефис
    "key=value",              // нет префикса
    "--key value",            // пробел вместо =
    "-- key=value",           // пробел после --
    "-1",                     // цифра вместо буквы
    "-=",                     // некорректный символ
    "",                       // пустая строка
    
    // Ваши оригинальные примеры
    "--z=l", 
    "--kkkk=1234",
    "--o=",
    "--help",
    "--H",
    "--",
    "---kkk",
    "-h",
    "-",
    "-=",
    "-a=z"
};
//   argvs = {
//     // Стандартные кейсы
//     "-a",
//     "--long",
//     "-abc",
//     "--key=value",
//     "--option=123",
//     "--flag=",
//     "-f=value",
//
//     // Граничные случаи
//     "",
//     "-",
//     "--",
//     "---",
//     "-=",
//     "--=value",
//     "-=value",
//     " -a",
//     "-- key=value",
//     "--key =value",
//     "--key= value",
//     "--key=val ue",
//
//     // Специальные символы
//     "--!@#$%",
//     "--key=!@#$%",
//     "--key=value1;value2",
//     "--key=value\nvalue",
//     "--key=\"value\"",
//     "--key='value'",
//
//     // Unicode и международные символы
//     "--ключ=значение",
//     "--東京=東京",
//     "--key=😊",
//     "--münchen=ümlaut",
//
//     // Некорректные форматы
//     "a",
//     "-1",
//     "--123=456",
//     "-a=b=c",
//     "--key value",
//     "key=value",
//     "-a -b",
//     "--key==value",
//
//     // Длинные значения
//     "--key=" + std::string(1000, 'a'),
//     // "-" + std::string(100, 'b'),
//     "--" + std::string(100, 'c'),
//
//     // Ваши оригинальные тесты
//     "--z=l",
//     "--kkkk=1234",
//     "--o=",
//     "--help",
//     "--H",
//     "--",
//     "---kkk",
//     "-h",
//     "-",
//     "-=",
//     "-a=z"
// };
#else
  if (argc == 1) { 
    //cout --help
    return EXIT_SUCCESS;
  }
  uint32_t index = 1u;
  while (index < argc) {
    argvs.push_back( *(argv + index) );
  }
#endif
  for (const std::string& fullArg : argvs) {
    auto parsedArg = parseArg(fullArg);
    if (parsedArg.size() == NULL) {
      co::error(static_cast<std::string>(NE_2) + " : \"" + fullArg + "\"");
#ifdef TEST_ARGS_PARSER
      continue;
#else
      return EXIT_FAILURE;
#endif
    }
#ifdef TEST_ARGS_PARSER
    for (const std::pair<std::string, std::string>& el : parsedArg) {
      printf("%s  :  %s", el.first.c_str(), el.second.c_str()); ENDLN;
    }
    continue;
#else
#endif
  }
  return EXIT_SUCCESS;
}

#undef IN_CODE_ARGS
#undef TEST_ARGS_PARSER
