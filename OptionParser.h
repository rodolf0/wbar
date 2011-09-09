#ifndef _OPTIONPARSER_
#define _OPTIONPARSER_

#include <string>

class OptionParser {
  public:
    OptionParser(int argc, char *argv[]);

    const std::string& getString(const std::string &opt) const;
    bool getBool(const std::string &opt) const;
};

#endif /* _OPTIONPARSER_ */

/* vim: set sw=2 sts=2 : */
