#include <dlfcn.h>

#include "nemu.h"
#include "monitor/monitor.h"
#include "diff-test.h"

static void (*ref_difftest_memcpy_from_dut)(paddr_t dest, void *src, size_t n);
static void (*ref_difftest_getregs)(void *c);
static void (*ref_difftest_setregs)(const void *c);
static void (*ref_difftest_exec)(uint64_t n);

static bool is_skip_ref;
static bool is_skip_dut;

void difftest_skip_ref() { is_skip_ref = true; }
void difftest_skip_dut() { is_skip_dut = true; }

void init_difftest(char *ref_so_file, long img_size) {
#ifndef DIFF_TEST
  return;
#endif

  assert(ref_so_file != NULL);

  void *handle;
  handle = dlopen(ref_so_file, RTLD_LAZY | RTLD_DEEPBIND);
  assert(handle);

  ref_difftest_memcpy_from_dut = dlsym(handle, "difftest_memcpy_from_dut");
  assert(ref_difftest_memcpy_from_dut);

  ref_difftest_getregs = dlsym(handle, "difftest_getregs");
  assert(ref_difftest_getregs);

  ref_difftest_setregs = dlsym(handle, "difftest_setregs");
  assert(ref_difftest_setregs);

  ref_difftest_exec = dlsym(handle, "difftest_exec");
  assert(ref_difftest_exec);

  void (*ref_difftest_init)(void) = dlsym(handle, "difftest_init");
  assert(ref_difftest_init);

  Log("Differential testing: \33[1;32m%s\33[0m", "ON");
  Log("The result of every instruction will be compared with %s. "
      "This will help you a lot for debugging, but also significantly reduce the performance. "
      "If it is not necessary, you can turn it off in include/common.h.", ref_so_file);

  ref_difftest_init();
  ref_difftest_memcpy_from_dut(ENTRY_START, guest_to_host(ENTRY_START), img_size);
  ref_difftest_setregs(&cpu);
}

void difftest_step(uint32_t eip) {
  CPU_state ref_r;

  if (is_skip_dut) {
    is_skip_dut = false;
    return;
  }

  if (is_skip_ref) {
    // to skip the checking of an instruction, just copy the reg state to reference design
    ref_difftest_setregs(&cpu);
    is_skip_ref = false;
    return;
  }

  ref_difftest_exec(1);
  ref_difftest_getregs(&ref_r);

  // TODO: Check the registers state with the reference design.
  // Set `nemu_state` to `NEMU_ABORT` if they are not the same.
  if(cpu.eax != ref_r.eax) {
		printf("eax should be %u, get %u\n", cpu.eax, ref_r.eax);
		nemu_state = NEMU_ABORT;
	}
	if(cpu.ecx != ref_r.ecx) {
		printf("ecx should be %u, get %u\n", cpu.ecx, ref_r.ecx);
		nemu_state = NEMU_ABORT;
	}
  if(cpu.edx != ref_r.edx) {
		printf("edx should be %u, get %u\n", cpu.edx, ref_r.edx);
		nemu_state = NEMU_ABORT;
	}
  if(cpu.ebx != ref_r.ebx) {
		printf("ebx should be %u, get %u\n", cpu.ebx, ref_r.ebx);
		nemu_state = NEMU_ABORT;
	}
  if(cpu.esp != ref_r.esp) {
		printf("esp should be %u, get %u\n", cpu.esp, ref_r.esp);
		nemu_state = NEMU_ABORT;
	}
  if(cpu.ebp != ref_r.ebp) {
		printf("ebp should be %u, get %u\n", cpu.ebp, ref_r.ebp);
		nemu_state = NEMU_ABORT;
	}
  if(cpu.esi != ref_r.esi) {
		printf("esi should be %u, get %u\n", cpu.esi, ref_r.esi);
		nemu_state = NEMU_ABORT;
	}
  if(cpu.edi != ref_r.edi) {
		printf("edi should be %u, get %u\n", cpu.edi, ref_r.edi);
		nemu_state = NEMU_ABORT;
	}
  if(cpu.eip != ref_r.eip) {
		printf("eip should be %u, get %u\n", cpu.eip, ref_r.eip);
		nemu_state = NEMU_ABORT;
	}	
	
}
