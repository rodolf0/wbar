#include <getopt.h>
#include <cstring>
#include <cstdlib>

#include "OptionParser.h"
#include "Options.h"

OptionParser::Option::Option(bool is_set_, int takes_arg_, const char *name_,
                             const char *def_val_, const char *descrip_,
                             const char *value_)
    : is_set(is_set_), takes_arg(takes_arg_), name(name_), def_val(def_val_),
      descrip(descrip_), value(value_) {}

OptionParser::OptionParser(int argc, char *argv[]) : options() {
  size_t num_opts = sizeof(cmdline_options) / sizeof(struct _cmdline_options);
  struct option opts[num_opts + 1];
  // prepare structures for getopt()
  std::memset(opts, 0, sizeof(opts));
  for (size_t i = 0; i < num_opts; i++) {
    opts[i].name = cmdline_options[i].name;
    opts[i].has_arg = cmdline_options[i].takes_arg;
    options.push_back(Option(
        false, cmdline_options[i].takes_arg,
        cmdline_options[i].name ? cmdline_options[i].name : "",
        cmdline_options[i].def_val ? cmdline_options[i].def_val : "",
        cmdline_options[i].descrip ? cmdline_options[i].descrip : "", ""));
  }

  for (int i; !getopt_long(argc, argv, "", opts, &i);) {
    options[i].is_set = true;
    if (optarg)
      options[i].value = optarg;
  }
}

OptionParser::~OptionParser() {}

const std::string &OptionParser::getString(const std::string &name) const {
  const Option &o = find(name);
  return (o.is_set ? o.value : o.def_val);
}

int OptionParser::getInt(const std::string &name) const {
  const Option &o = find(name);
  return std::atoi(o.is_set ? o.value.c_str() : o.def_val.c_str());
}

float OptionParser::getFloat(const std::string &name) const {
  const Option &o = find(name);
  return std::atof(o.is_set ? o.value.c_str() : o.def_val.c_str());
}

bool OptionParser::isset(const std::string &name) const {
  const Option &o = find(name);
  return o.is_set;
}

const OptionParser::Option &OptionParser::find(const std::string &name) const {
  for (std::vector<Option>::const_iterator it = options.begin();
       it != options.end(); it++) {
    if (it->name == name)
      return *it;
  }
  throw "Unkown option name.";
}

/* vim: set sw=2 sts=2 : */
