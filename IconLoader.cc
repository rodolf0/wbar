extern "C" {
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
};

#include "IconLoader.h"
#include "debug.h"

using namespace std;

IconLoader::IconLoader(const char *cfgFile) : configFile(cfgFile){

    string homeDir;
    struct stat cfg_stat;


    if(configFile == "-"){

	homeDir = getenv("HOME");
	if( !stat((homeDir + "/.wbar").c_str(), &cfg_stat) )
	    configFile = homeDir + "/.wbar";

	else if( !stat("dot.wbar", &cfg_stat) )
	    configFile = "dot.wbar";

	else if( !stat("/usr/share/wbar/dot.wbar", &cfg_stat) )
	    configFile = "/usr/share/wbar/dot.wbar";

	else 
	    throw "Can't stat a config file. Try # make install or -config <cfgfile>";
    }

    ERRMSG("Using " << configFile << " config file.");

    regcomp(&icon, "^[[:space:]]*[^#]*i:[[:space:]]*(.*)",
	REG_EXTENDED);
    regcomp(&comm, "^[[:space:]]*[^#]*c:[[:space:]]*(.*)", 
	REG_EXTENDED);
    regcomp(&text, "^[[:space:]]*[^#]*t:[[:space:]]*(.*)", 
	REG_EXTENDED);
}

IconLoader::~IconLoader(){
    regfree(&icon);
    regfree(&comm);
    regfree(&text);
}

void IconLoader::openConf(){
    cfgStream.open(configFile.c_str());

    if( !cfgStream )
	throw (configFile + " -> Couldn't open config file.").c_str();
}

void IconLoader::closeConf(){
    cfgStream.close();
}

int IconLoader::nextIconInfo(string &img, string &cmd, string &txt){
    char buffer[256];
    int elem = 0;

    /* This does no error check. 
     * When 3 elements r pickd up they get returned 
     * therefore they may be scrambled but just among the 3 
     * i: t: c: */

    while( cfgStream.getline(buffer, sizeof(buffer)) ){

	/* Icon */
	if(!regexec(&icon, buffer, 2, pmatch, 0)){
	    img = /* homeDir + "/" +  */
		string(buffer + pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so);
	    elem++;
	/* Command */
	}else if(!regexec(&comm, buffer, 2, pmatch, 0)){
	    cmd = string(buffer + pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so);
	    elem++;
	/* Text */
	}else  if(!regexec(&text, buffer, 2, pmatch, 0)){
	    txt = string(buffer + pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so);
	    elem++;
	}

	if(elem >= 3)
	    return 0;
    }

    return 1;
}
