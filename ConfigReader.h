#ifndef _CONFIGREADER_
#define _CONFIGREADER_

#include <map>
#include <list>
#include <string>

class ConfigReader {
public:
  ConfigReader(const std::string &path);

  class Section {
  public:
    Section(const std::string &name);
    void add(const std::string &key, const std::string &value);
    const std::string &get(const std::string &key) const;
    const std::string &name() const;

  protected:
    std::string _name;
    std::map<std::string, std::string> entries;
  };

  const Section &get(const std::string &name) const;

  // section iterators
  std::list<Section>::const_iterator begin() const;
  std::list<Section>::const_iterator end() const;

protected:
  std::list<Section> sections;
};

#endif /* _CONFIGREADER_ */

/* vim: set sw=2 sts=2 : */
