#include <am.h>
#include <nemu.h> 

#define SYNC_ADDR (VGACTL_ADDR + 4)
//static AM_GPU_CONFIG_T gpu;
static uint32_t wid, hei;
void __am_gpu_init() {
  uint32_t wh = inl(VGACTL_ADDR);
  hei = wh & 0xFFFF;
  wid = (wh >> 16) & 0xFFFF;
  int i;
  int w = wid;  // TODO: get the correct width
  int h = hei;  // TODO: get the correct height
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);

}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = wid, .height = hei,
    .vmemsz = wid * hei * sizeof(uint32_t)
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  if (ctl->sync) {//if the software requires to sync, it write SYNC_ADDR with 1
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
