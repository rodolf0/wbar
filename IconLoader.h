#ifndef _ICONLOADER_H_
#define _ICONLOADER_H_

extern "C" {
#include <sys/types.h>
#include <regex.h>
};

#include <fstream>
#include <string>

class IconLoader{
    private:
	std::ifstream cfgStream;

	std::string configFile;
	
	regex_t icon, text, comm;
	regmatch_t pmatch[2];

    public:

	IconLoader(const char *cfgFile);
	~IconLoader();

	void openConf();
	void closeConf();

	int nextIconInfo(std::string &img, std::string &cmd, std::string &str);

};

#endif /* _ICONLOADER_H_ */

