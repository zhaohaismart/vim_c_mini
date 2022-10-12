#include <stdio.h>
#include <uv.h>

static uv_mutex_t delay_mutex;
static uv_cond_t delay_cond;

void time_init(void) {
  uv_mutex_init(&delay_mutex);
  uv_cond_init(&delay_cond);
}
int main() {
  time_init(); 
  return 0;
}
