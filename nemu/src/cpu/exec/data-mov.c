#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
	uint32_t tt;
  rtl_push(&(id_dest->val));
	rtl_top(&tt);
  print_asm_template1(push);
}

make_EHelper(pop) {
	uint32_t tt;
	rtl_top(&tt);
  rtl_pop(&(id_dest->val));
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
	Log("before leave the ESP is %x", reg_l(R_ESP));
  rtl_mv(&reg_l(R_ESP), &reg_l(R_EBP));
	rtl_pop(&reg_l(R_EBP));
	Log("after leave the ESP is %x", reg_l(R_ESP));
  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t0, &id_src->val, id_src->width);
  operand_write(id_dest, &t0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
	/*if(id_dest->width == 2)
		Log("before movzxw %u", reg_w(id_dest->reg));
	else
		Log("before movzxl %u", reg_l(id_dest->reg));*/
  operand_write(id_dest, &id_src->val);
	/*if(id_dest->width == 2)
		Log("after movzxw %u", reg_w(id_dest->reg));
	else
		Log("after movzxl %u", reg_l(id_dest->reg));*/
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
	if(id_dest->width == 2)
		Log("leaw is %u", reg_w(id_dest->reg));
	else
		Log("leal is %u", reg_l(id_dest->reg));
  print_asm_template2(lea);
}
