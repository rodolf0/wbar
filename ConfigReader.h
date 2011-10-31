#ifndef _CONFIGREADER_
#define _CONFIGREADER_

#include <map>
#include <string>

typedef std::map<std::string, std::string> section_t;

class ConfigReader {
  public:

    ConfigReader(const std::string &path);

    const section_t & section(std::string section);
    const std::string & get(std::string section, std::string key);

  protected:

    std::map<std::string, section_t> config;
};

#endif /* _CONFIGREADER_ */

/* vim: set sw=2 sts=2 : */
