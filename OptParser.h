#ifndef _OPTPARSER_
#define _OPTPARSER_

#define MAXOPTS 20
#include <getopt.h>
#include <string>

class OptParser{
    private:
	struct realOpts{
	    int isOptionSet;
	    std::string argument;
	};

	struct option longOption[MAXOPTS];
	struct realOpts setOption[MAXOPTS];

	int optCount;

    public:

	OptParser();

	void addOption(const char *name, bool hasArg, const char *defArg=NULL);
	void parse(int argc, char * const argv[]);

	/* returns option index */
	int isset(std::string name);
	std::string getArg(std::string name);


};

#endif /* _OPTPARSER_ */
