#include <fstream>
#include <regex.h>
#include "ConfigReader.h"

#define SUBREGS 3
#define LINEBUF 256
ConfigReader::ConfigReader(const std::string &path) :
    sections() {
  regex_t section_expr, value_expr;
  regmatch_t match_info[SUBREGS];
  std::ifstream cfgfile;
  char linebuf[LINEBUF];

  if (regcomp(&section_expr, "^\\[(.+)\\]$", REG_EXTENDED))
    throw "Failed to compile section regex.";
  if (regcomp(&value_expr, "^ *([^= ]+) *= *(.*) *$", REG_EXTENDED))
    throw "Failed to compile value regex.";

  cfgfile.open(path.c_str());
  if (cfgfile.rdstate() != std::ios_base::goodbit)
    throw "Failed to open config file.";

  while (!cfgfile.getline(linebuf, sizeof(linebuf)).eof()) {

    if (!regexec(&section_expr, linebuf, SUBREGS, match_info, 0)) {
      sections.push_back(
        std::string(linebuf + match_info[1].rm_so,
                    match_info[1].rm_eo - match_info[1].rm_so));

    } else if (!regexec(&value_expr, linebuf, SUBREGS, match_info, 0)) {
      sections.back().add(
        std::string(linebuf + match_info[1].rm_so,
                    match_info[1].rm_eo - match_info[1].rm_so),
        std::string(linebuf + match_info[2].rm_so,
                    match_info[2].rm_eo - match_info[2].rm_so));
    }
  }
  cfgfile.close();
  regfree(&section_expr);
  regfree(&value_expr);
}


std::list<ConfigReader::Section>::const_iterator ConfigReader::begin() const {
  return sections.begin();
}

std::list<ConfigReader::Section>::const_iterator ConfigReader::end() const {
  return sections.end();
}

const ConfigReader::Section & ConfigReader::get(const std::string &name) const {
  for (std::list<Section>::const_iterator s = sections.begin();
       s != sections.end(); s++) {
    if (s->name() == name)
      return *s;
  }
  throw "ERROR: invalid section name.";
}


/////////////// ConfigReader::Section ////////////////

ConfigReader::Section::Section(const std::string &name_) :
    _name(name_), entries() { }

void ConfigReader::Section::add(const std::string &k, const std::string &v) {
  entries.insert(std::make_pair(k, v));
}

const std::string & ConfigReader::Section::get(const std::string &key) const {
  std::map<std::string, std::string>::const_iterator e = entries.find(key);
  if (e != entries.end())
    return e->second;
  throw "ERROR: invalid key name.";
}

const std::string & ConfigReader::Section::name() const {
  return _name;
}

/* vim: set sw=2 sts=2 : */
