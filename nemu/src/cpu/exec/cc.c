#include "cpu/rtl.h"

/* Condition Code */

void rtl_setcc(rtlreg_t* dest, uint8_t subcode) {
  bool invert = subcode & 0x1;
  enum {
    CC_O, CC_NO, CC_B,  CC_NB,
    CC_E, CC_NE, CC_BE, CC_NBE,
    CC_S, CC_NS, CC_P,  CC_NP,
    CC_L, CC_NL, CC_LE, CC_NLE
  };
	uint32_t re;
  // TODO: Query EFLAGS to determine whether the condition code is satisfied.
  // dest <- ( cc is satisfied ? 1 : 0)
  switch (subcode & 0xe) {
    case CC_O:
		case CC_NO:
    case CC_B:
		case CC_NB:
    case CC_E: {
				rtl_get_ZF(&re);
				if(re)
					*dest = 1;
				else
					*dest = 0;
			}
			break;
		case CC_NE:
    case CC_BE:
		case CC_NBE:
    case CC_S:
		case CC_NS:
		case CC_NP:
		case CC_L:
		case CC_NL:
    case CC_LE:
		case CC_NLE:
    default: panic("should not reach here");
    case CC_P: panic("n86 does not have PF");
  }

  if (invert) {
    rtl_xori(dest, dest, 0x1);
  }
}
