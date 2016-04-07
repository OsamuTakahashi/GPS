#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

void init_lexertest(void);
void init_parsertest(void);
void init_utiltest(void);

int main(int argc,char **argv) {
  CU_initialize_registry();

  init_lexertest();
  init_parsertest();
  init_utiltest();

  CU_basic_run_tests();
  CU_cleanup_registry();
  return 0;
}
