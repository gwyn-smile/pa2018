#include "cpu/exec.h"

void difftest_skip_ref();
void difftest_skip_dut();

make_EHelper(lidt) {
  TODO();

  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

make_EHelper(int) {
  TODO();

  print_asm("int %s", id_dest->str);

#if defined(DIFF_TEST) && defined(DIFF_TEST_QEMU)
  difftest_skip_dut();
#endif
}

make_EHelper(iret) {
  TODO();

  print_asm("iret");
}

make_EHelper(in) {
	Log("the in instr:the width is %d", id_dest->width); 
  assert(id_dest->width == 1 || id_dest->width == 2 || id_dest->width == 4);
	if(ioperm(id_src->val, 1, 1) != 0) {
		perror("ioperm");
		assert(0);
	}
	if(id_dest->width == 1) {
		id_dest->val = inb(id_src->val);
		rtl_mv(&reg_l(id_dest->reg), &id_dest->val);
	}
	else if(id_dest->width == 2) {
		id_dest->val = inw(id_src->val);
		rtl_mv(&reg_l(id_dest->reg), &id_dest->val);
	}
	else {
		id_dest->val = inl(id_src->val);
		rtl_mv(&reg_l(id_dest->reg), &id_dest->val);
	}
  print_asm_template2(in);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

make_EHelper(out) {
  TODO();

  print_asm_template2(out);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}
