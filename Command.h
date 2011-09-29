#ifndef _COMMAND_
#define _COMMAND_

#include <string>

class Command {
  public:
    virtual ~Command() = 0;
};


class ExecuteCommand : public Command {
  public:
    ExecuteCommand(const std::string &comm);
};

#endif /* _COMMAND_ */

/* vim: set sw=2 sts=2 : */
