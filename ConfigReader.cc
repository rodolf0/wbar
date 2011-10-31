#include <fstream>
#include <regex.h>
#include "ConfigReader.h"

#define SUBREGS 3
ConfigReader::ConfigReader(const std::string &path) {
  regex_t section_expr, value_expr;
  regmatch_t match_info[SUBREGS];
  char linebuf[256];
  std::ifstream cfgfile;

  if (regcomp(&section_expr, "^\\[(\\w+)\\]$", REG_EXTENDED))
    throw "Failed to compile section regex.";
  if (regcomp(&value_expr, "^ *([^= ]+) *= *(.*) *$", REG_EXTENDED))
    throw "Failed to compile value regex.";

  std::string section(""); // default section for key-values
  cfgfile.open(path.c_str());
  while (!cfgfile.getline(linebuf, sizeof(linebuf)).eof()) {
    if (!regexec(&section_expr, linebuf, SUBREGS, match_info, 0)) {
      section = std::string(linebuf + match_info[1].rm_so,
                            match_info[1].rm_eo - match_info[1].rm_so);
    } else if (!regexec(&value_expr, linebuf, SUBREGS, match_info, 0)) {
      std::string key(linebuf + match_info[1].rm_so,
                      match_info[1].rm_eo - match_info[1].rm_so);
      std::string value(linebuf + match_info[2].rm_so,
                        match_info[2].rm_eo - match_info[2].rm_so);
      config[section][key] = value;
    }
  }
  cfgfile.close();
  regfree(&section_expr);
}

const section_t & ConfigReader::section(std::string section) {
  if (config.count(section))
    return config[section];
  throw "Config section not found.";
}

const std::string & ConfigReader::get(std::string section, std::string key) {
  section_t s = this->section(section);
  if (s.count(key))
    return s[key];
  throw "Config key not found.";
}

/* vim: set sw=2 sts=2 : */
