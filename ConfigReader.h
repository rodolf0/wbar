#ifndef _CONFIGREADER_
#define _CONFIGREADER_

#include <string>
#include <list>

class ConfigReader {
  public:

    class ConfigSection {
      public:
        const std::string& getString(const std::string &var) const;
        const std::string& getName() const;
    };

  private:
  public:

    typedef std::list<ConfigSection>::const_iterator SectionIterator;
    std::list<ConfigSection> sections;

    ConfigReader(const std::string &path);

    const ConfigSection& getSection(const std::string &sec) const;


};

#endif /* _CONFIGREADER_ */

/* vim: set sw=2 sts=2 : */
