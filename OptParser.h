#ifndef _OPTPARSER_
#define _OPTPARSER_

#include <vector>

#define ARG_SIZE 32

class Bar;

class OptParser {
    public:

        OptParser(int argc, char *argv[]);

        int isSet(const char *name) const;
        int takesArg(const char *name) const;
        const char *getArgument(const char *name) const;
        const char *getDefault(const char *name) const;
        void printDescrip() const;

        void configure(Bar *);
};

class ConfigParser {
    public:

        ConfigParser(const char *cfg);

        const char *getWidget(int index) const;
        const char *getCommand(int index) const;
        const char *getCaption(int index) const;
        int widgetCount() const;

    private:
        std::vector<std::string> widget;
        std::vector<std::string> command;
        std::vector<std::string> caption;
};

#endif /* _OPTPARSER_ */
