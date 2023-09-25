/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include <memory/host.h>
#include <memory/vaddr.h>
#include <device/map.h>

#define IO_SPACE_MAX (2 * 1024 * 1024)

static uint8_t *io_space = NULL; //io mapper memory address
static uint8_t *p_space = NULL;//physical memory address 

//io new space malloc function
uint8_t* new_space(int size) { //malloc size bytes in the physical space
  uint8_t *p = p_space;
  // page aligned;
  size = (size + (PAGE_SIZE - 1)) & ~PAGE_MASK;
  p_space += size;
  assert(p_space - io_space < IO_SPACE_MAX);//the space memory size doesn't exceed
  return p;//return the new space begin pointer
}

//check address with the map struct
static void check_bound(IOMap *map, paddr_t addr) {
  if (map == NULL) {
    Assert(map != NULL, "address (" FMT_PADDR ") is out of bound at pc = " FMT_WORD, addr, cpu.pc);
  } else {
    Assert(addr <= map->high && addr >= map->low,
        "address (" FMT_PADDR ") is out of bound {%s} [" FMT_PADDR ", " FMT_PADDR "] at pc = " FMT_WORD,
        addr, map->name, map->low, map->high, cpu.pc);
  }
}

//call the callback function 
//io_callback_t is a function pointer, with input uint32_t, int and bool
static void invoke_callback(io_callback_t c, paddr_t offset, int len, bool is_write) {
  if (c != NULL) { c(offset, len, is_write); }
}

//malloc IO_SPACE_MAX size address memory space for io to mmio
void init_map() {
  io_space = malloc(IO_SPACE_MAX);
  assert(io_space);
  p_space = io_space; //p_space begin with io_space
}

//map_read will map the memory address : addr to the IOMap *map's target space, and will access 
word_t map_read(paddr_t addr, int len, IOMap *map) {
  assert(len >= 1 && len <= 8); //read len size check
  check_bound(map, addr); //check bound, assure addr is in the address space of map
  paddr_t offset = addr - map->low; //offset from the map->low
  #ifdef CONFIG_DTRACE
    Log("[DTRACE]: Access Device %s, Read 0x%08x bytes @ address 0x%08x", map->name, (uint32_t)len, (uint32_t)addr);
  #endif

  invoke_callback(map->callback, offset, len, false); // prepare data to read
  word_t ret = host_read(map->space + offset, len);
  return ret; //return read bytes by host_read
}
 
//same as map_read
void map_write(paddr_t addr, int len, word_t data, IOMap *map) {
  assert(len >= 1 && len <= 8);
  check_bound(map, addr);
  paddr_t offset = addr - map->low;
  #ifdef CONFIG_DTRACE
    Log("[DTRACE]: Access Device %s, Write 0x%08x bytes data 0x%08x @ address 0x%08x", map->name, (uint32_t)len, (uint32_t)data, (uint32_t)addr);
  #endif
  host_write(map->space + offset, len, data);
  invoke_callback(map->callback, offset, len, true);
}
