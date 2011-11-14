#ifndef _OPTIONS_
#define _OPTIONS_

enum argtype {
  no_arg = 0,
  req_arg,
  opt_arg,
};

struct _cmdline_options {
  const char *name;
  argtype    takes_arg;
  const char *descrip;
  const char *def_val;
} cmdline_options[] = {

  { "help"       , no_arg  , "this help" }                                ,
  { "config"     , req_arg , "config file"                                , "wbar.cfg" } ,
  { "size"       , req_arg , "widget normal size"                         , "32" }       ,
  { "zoomf"      , req_arg , "widget zoomed size factor"                  , "1.8" }      ,
  { "jumpf"      , req_arg , "widget jump factor"                         , "1.0" }      ,
  { "nanim"      , req_arg , "number of animated icons"                   , "3" }        ,
  { "layer"      , req_arg , "above|bellow choose at wich layer to run" } ,
  { "above-desk" , no_arg  , "run over desktop manager (ie: nautilus)" }  ,
  { "pos"        , req_arg , "wbar position (top / bottom / +x+y)"        , "bottom" }   ,

};

#endif /* _OPTIONS_ */

/* vim: set sw=2 sts=2 : */
