#include "cpu/exec.h"
#include "cpu/cc.h"
const uint32_t eflags_1 = 1;
const uint32_t eflags_0 = 0;
make_EHelper(test) {
	rtl_and(&id_dest->val, &id_dest->val, &id_src->val);
	//eflags
	rtl_set_OF(&eflags_0);
	rtl_set_CF(&eflags_0);
	rtl_update_ZFSF(&id_dest->val, id_dest->width);
  
	print_asm_template2(test);
}

make_EHelper(and) {
	//Log("before and %08x", reg_l(id_dest->reg));
	if(id_src->type == OP_TYPE_IMM) {
		rtl_and_i(&id_dest->val, &id_dest->val, &id_src->val);
	}
	else {
		rtl_and(&id_dest->val, &id_dest->val, &id_src->val); 
	}
	//Log("after and %08x", reg_l(id_dest->reg));
  
	operand_write(id_dest, &id_dest->val);
	//eflags
	rtl_set_OF(&eflags_0);
	rtl_set_CF(&eflags_1);

	rtl_update_ZFSF(&id_dest->val, 4);
  
	print_asm_template2(and);
}

make_EHelper(xor) {
	//Log("before xor %08x", reg_l(id_dest->reg));
	if(id_src->type == OP_TYPE_IMM) {
		rtl_xor_i(&id_dest->val, &id_dest->val, &id_src->val);
	}
	else {
		rtl_xor(&id_dest->val, &id_dest->val, &id_src->val);
	}
	//Log("after xor %08x", reg_l(id_dest->reg));
  
	operand_write(id_dest, &id_dest->val);
	//eflags
	rtl_set_OF(&eflags_0);
	rtl_set_CF(&eflags_0);

	rtl_update_ZFSF(&id_dest->val, id_dest->width);

  print_asm_template2(xor);
}

make_EHelper(or) {
  rtl_or(&id_dest->val, &id_dest->val, &id_src->val);
  
	operand_write(id_dest, &id_dest->val);
	//eflags
	rtl_set_OF(&eflags_0);
	rtl_set_CF(&eflags_0);

	rtl_update_ZFSF(&id_dest->val, id_dest->val);

  print_asm_template2(or);
}

make_EHelper(sar) {
  // unnecessary to update CF and OF in NEMU
	rtl_sar(&id_dest->val, &id_dest->val, &id_src->val);
	operand_write(id_dest, &id_dest->val);
  print_asm_template2(sar);
}

make_EHelper(shl) {
  // unnecessary to update CF and OF in NEMU
	rtl_shl(&id_dest->val, &id_dest->val, &id_src->val);
	operand_write(id_dest, &id_dest->val);
  print_asm_template2(shl);
}

make_EHelper(shr) {
  // unnecessary to update CF and OF in NEMU
	rtl_shr(&id_dest->val, &id_dest->val, &id_src->val);
	operand_write(id_dest, &id_dest->val);
  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decoding.opcode & 0xf;

  rtl_setcc(&t2, cc);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
	rtl_not(&id_dest->val, &id_dest->val);
	operand_write(id_dest, &id_dest->val);
  print_asm_template1(not);
}
