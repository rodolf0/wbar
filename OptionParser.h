#ifndef _OPTIONPARSER_
#define _OPTIONPARSER_

#include <string>
#include <vector>

class OptionParser {
  public:
    OptionParser(int argc, char *argv[]);
    virtual ~OptionParser();

    const std::string& getString(const std::string &name) const;
    int getInt(const std::string &name) const;
    bool isset(const std::string &name) const;

  protected:

    class Option {
      public:
        bool  is_set;
        int   takes_arg;
        std::string name;
        std::string def_val;
        std::string descrip;
        std::string argument;

        Option(bool is_set, int takes_arg,
               const char *name, const char *def_val,
               const char *descrip, const char *argument);
    };

    std::vector<Option> options;
    const Option & find(const std::string &name) const;
};

#endif /* _OPTIONPARSER_ */

/* vim: set sw=2 sts=2 : */
