#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <iostream>

#define DEBUG(x) \
    (std::cout << __FILE__ << " line " << __LINE__ << \
     " (" << __FUNCTION__ << "); Var: " << #x << " Val = " << x << std::endl << std::endl)

#define ERRMSG(x) (std::cout << x << std::endl)

#endif /* _DEBUG_H_ */
