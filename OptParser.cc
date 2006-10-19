#include "OptParser.h"

using namespace std;

OptParser::OptParser() : optCount(0) {}

void OptParser::addOption(const char *name, bool hasArg, const char *defArg){
    if(optCount >= MAXOPTS)
	throw "Max numer of options reached";

    /* they will be harcoded strings so dont alloc them */
    longOption[optCount].name = name;
    longOption[optCount].has_arg = hasArg;
    longOption[optCount].flag = 0;
    longOption[optCount].val = 0;

    if(defArg)
	setOption[optCount].argument = defArg;
    setOption[optCount].isOptionSet = (hasArg && defArg != NULL)? 2:0;

    optCount++;
}

void OptParser::parse(int argc, char * const argv[]){
    int optIndex, c;

    while( (c=getopt_long_only(argc, argv, "h", longOption, &optIndex)) != -1 )
	if(c == 0){
	    setOption[optIndex].isOptionSet = 1;
	    if(optarg)
		setOption[optIndex].argument = optarg;
	}
}

int OptParser::isset(string name){

    for(int i=0; i<optCount; i++)
	if(longOption[i].name == name)
	    if(setOption[i].isOptionSet == 1)
		return 1;

    return 0;
}

string OptParser::getArg(string name){

    for(int i=0; i<optCount; i++)
	if(longOption[i].name == name)
	    if(setOption[i].isOptionSet != 0)
		return setOption[i].argument;

    return NULL;
}
