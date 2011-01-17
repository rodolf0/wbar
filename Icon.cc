#include "Icon.h"

Icon::Icon(const char *iconImg, const char *cmd) :
  icon( 1, 1 ), bx(0), by(0), bs(0), command(cmd) {

  icon = Image( iconImg );
  size = icon.ow;
}

Image& Icon::render() {
  return icon.full();
}

Icon::~Icon() {}
