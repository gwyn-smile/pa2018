#include "cpu/exec.h"
#include "cpu/cc.h"
const uint32_t eflags_1 = 1;
const uint32_t eflags_0 = 0;
make_EHelper(test) {
  TODO();

  print_asm_template2(test);
}

make_EHelper(and) {
	if(id_src->type == OP_TYPE_IMM) {
		//Log("before and %08x", reg_l(id_dest->reg));
		rtl_and_i(&id_dest->val, &id_dest->val, &id_src->val);
		rtl_mv(&reg_l(id_dest->reg), &id_dest->val);
		//Log("after and %08x", reg_l(id_dest->reg));
	}
	else {
		//Log("before and %08x", reg_l(id_dest->reg));
		rtl_and(&id_dest->val, &id_dest->val, &id_src->val); 
		rtl_mv(&reg_l(id_dest->reg), &id_dest->val);
		//Log("after and %08x", reg_l(id_dest->reg));
	}
	//eflags
	rtl_set_OF(&eflags_0);
	rtl_set_CF(&eflags_1);

	rtl_update_ZFSF(&id_dest->val, 4);
  
	print_asm_template2(and);
}

make_EHelper(xor) {
	if(id_src->type == OP_TYPE_IMM) {
		rtl_xor_i(&(id_dest->val), &(id_dest->val), &(id_src->val));
		rtl_mv(&reg_l(id_dest->reg), &id_dest->val);	
	}
	else {
		//Log("before xor %08x", reg_l(id_dest->reg));
		rtl_xor(&(id_dest->val), &(id_dest->val), &(id_src->val));
		rtl_mv(&reg_l(id_dest->reg), &id_dest->val);
		//Log("after xor %08x", reg_l(id_dest->reg));
	}
	//eflags
	rtl_set_OF(&eflags_0);
	rtl_set_CF(&eflags_0);

	rtl_update_ZFSF(&id_dest->val, 4);

  print_asm_template2(xor);
}

make_EHelper(or) {
  TODO();

  print_asm_template2(or);
}

make_EHelper(sar) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
}

make_EHelper(shl) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}

make_EHelper(shr) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decoding.opcode & 0xf;

  rtl_setcc(&t2, cc);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  TODO();

  print_asm_template1(not);
}
