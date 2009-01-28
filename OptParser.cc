extern "C" {
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <regex.h>
};

#include <iostream>
#include <fstream>
#include "OptParser.h"


static struct _op_options {
    const char *name;
    int  takes_arg;
    const char *def_val;
    const char *descrip;
    int  is_set;
    char argument[ARG_SIZE];
} op_options[] = {

    {"help"         , 0, ""             , "this help"},
    {"config"       , 0, "-"            , "config file"},
    {"above-desk"   , 0, ""             , "run over desktop manager (ie: nautilus)"},
    {"isize"        , 1, "32"           , "icon normal size"},
    {"idist"        , 1, "1"            , "icon spacing"},
    {"zoomf"        , 1, "1.8"          , "icon grow factor"},
    {"jumpf"        , 1, "1"            , "icon jump factor"},
    {"pos"          , 1, "bottom"       , "wbar position (top, left, bot-right, etc.)"},
    {"balfa"        , 1, "-1"           , "bar alpha value"},
    {"falfa"        , 1, "-1"           , "lost focus bar alpha"},
    {"dblclk"       , 1, "200"          , "double click time in ms"},
    {"bpress"       , 0, ""             , "3D press simulation"},
    {"vbar"         , 0, ""             , "vertical wbar"},
    {"filter"       , 1, "0"            , "filter designation (0:none, 1:hovered, 2:others)"},
    {"fc"           , 1, "0xff00c800"   , "filter color 0xAARRGGBB"},
    {"nanim"        , 1, "7"            , "number of animated icons"},
    {"nofont"       , 0, ""             , "disable font rendering"},
    {"startdelay"   , 1, "15"           , "startup delay"}
};


OptParser::OptParser(int argc, char *argv[]) {
    int i;
    struct option longOpts[ sizeof(op_options) / sizeof(struct _op_options) ];

    memset(longOpts, 0x00, sizeof(longOpts));
    for( i = 0; i < (int)(sizeof(op_options) / sizeof(struct _op_options)); i++ ) {
        longOpts[i].name = op_options[i].name;
        longOpts[i].has_arg = op_options[i].takes_arg;
    }

    while( getopt_long_only(argc, argv, "h", longOpts, &i) == 0 ) {
        op_options[i].is_set = 1;
        if(optarg) {
            strncpy(op_options[i].argument, optarg, ARG_SIZE-1);
            op_options[i].argument[ARG_SIZE-1] = '\0';
        }
    }
}


int OptParser::isSet(const char *name) const {
    int i;

    for( i = 0; i < (int)(sizeof(op_options) / sizeof(struct _op_options)); i++)
        if( strcmp(op_options[i].name, name) == 0 )
            return op_options[i].is_set;

    return -1;
}

int OptParser::takesArg(const char *name) const {
    int i;

    for( i = 0; i < (int)(sizeof(op_options) / sizeof(struct _op_options)); i++)
        if( strcmp(op_options[i].name, name) == 0 )
            return op_options[i].takes_arg;

    return -1;
}

const char *OptParser::getArgument(const char *name) const {
    int i;

    for( i = 0; i < (int)(sizeof(op_options) / sizeof(struct _op_options)); i++)
        if( strcmp(op_options[i].name, name) == 0 )
            return (op_options[i].is_set) ? op_options[i].argument : op_options[i].def_val;

    return NULL;
}

const char *OptParser::getDefault(const char *name) const {
    int i;

    for( i = 0; i < (int)(sizeof(op_options) / sizeof(struct _op_options)); i++)
        if( strcmp(op_options[i].name, name) == 0 )
            return op_options[i].def_val;

    return NULL;
}

void OptParser::printDescrip() const {
    int i;

    std::cout << "Usage: wbar [options]" << std::endl;
    std::cout << "Options:" << std::endl << std::endl;

    std::cout.width(12);

    for( i = 0; i < (int)(sizeof(op_options) / sizeof(struct _op_options)); i++){
        std::cout.width(14);
        std::cout.setf(std::ios::left);
        std::cout << op_options[i].name;
        std::cout.width(0);
        std::cout << ": " << op_options[i].descrip << std::endl;
    }
}

ConfigParser::ConfigParser(const char *cfg) {

    struct stat ss;
    std::ifstream cfg_file;
    regex_t r_widget, r_command, r_caption;
    regmatch_t pmatch[2];
    char buffer[256];

    if( std::string("-") == std::string(cfg) ) {

        if( !stat((std::string( getenv("HOME"))+"/.wbar" ).c_str(), &ss) )
            cfg_file.open((std::string( getenv("HOME"))+"/.wbar" ).c_str());

        else if( !stat("dot.wbar", &ss) )
            cfg_file.open("dot.wbar");

        else if( !stat("/usr/share/wbar/dot.wbar", &ss) )
            cfg_file.open("/usr/share/wbar/dot.wbar");

    }else if( !stat(cfg, &ss) )
        cfg_file.open(cfg);
    else
        throw "No config file found";

    regcomp(&r_widget, "^[[:space:]]*[^#]*i:[[:space:]]*(.*)", REG_EXTENDED);
    regcomp(&r_command, "^[[:space:]]*[^#]*c:[[:space:]]*(.*)", REG_EXTENDED);
    regcomp(&r_caption, "^[[:space:]]*[^#]*t:[[:space:]]*(.*)", REG_EXTENDED);

    while( cfg_file.getline(buffer, sizeof(buffer)) ) {

        if( !regexec(&r_widget, buffer, 2, pmatch, 0) ) {
            widget.push_back( std::string(buffer + pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so) );

        }else if( !regexec(&r_command, buffer, 2, pmatch, 0) ) {
            command.push_back( std::string(buffer + pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so) );

        }else if( !regexec(&r_caption, buffer, 2, pmatch, 0) ) {
            caption.push_back( std::string(buffer + pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so) );
        }
    }

    cfg_file.close();

    regfree( &r_widget );
    regfree( &r_command );
    regfree( &r_caption );
}

const char *ConfigParser::getWidget(int index) const { return widget[index].c_str(); }
const char *ConfigParser::getCommand(int index) const { return command[index].c_str(); }
const char *ConfigParser::getCaption(int index) const { return caption[index].c_str(); }
int ConfigParser::widgetCount() const { return widget.size(); }
