// =============================
// Project: pico_fpga_timing — matched to evolvablehardware/ice40_memory_controller UART protocol
// =============================
// This canvas contains multiple files. Create them with the same relative paths.
//
// ├─ CMakeLists.txt
// ├─ pico_sdk_import.cmake        (symlink/copy from your SDK install)
// └─ src/
//    ├─ main.c                    (CLI + runners for BRAM/SPRAM, warmboot)
//    ├─ protocol.h               (opcode assembly per repo README)
//    ├─ transport_uart.c         (raw UART byte stream per README)
//    ├─ transport.h              (API)
//    └─ util.c / util.h          (timers, csv, rng)
//
// Notes:
//  • Protocol derived from repo README "Command Structure" and BRAM/SPRAM sections.
//  • Packet is a raw byte stream (no start/end markers). Ordering matches README:
//      op_byte, addr(1 for BRAM | 2 for SPRAM), size_byte (N-1), then 2*N data bytes for WR; for RD, FPGA returns 2*N bytes.
//  • Words are 16-bit. Size supports up to 256 (we send N-1 to represent 1..256).
//  • Warmboot: op byte with bit5=1; bits1:0 select image. (No payload expected.)
//
// =============================
// File: CMakeLists.txt
// =============================
cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(pico_fpga_timing C CXX ASM)
pico_sdk_init()

add_executable(pico_fpga_timing
    src/main.c
    src/transport_uart.c
    src/util.c
)

# Logging over UART0 @115200
pico_enable_stdio_uart(pico_fpga_timing 1)
pico_enable_stdio_usb(pico_fpga_timing 0)

# Libraries
target_link_libraries(pico_fpga_timing
    pico_stdlib
    hardware_uart
)

# Compile-time options (override with -D at cmake)
target_compile_definitions(pico_fpga_timing PRIVATE
    UART_LOG_BAUD=115200
    UART1_BAUD=2000000          # FPGA link over UART1
    PIN_UART1_TX=8              # GP8  -> FPGA RX
    PIN_UART1_RX=9              # GP9  -> FPGA TX
)

pico_add_extra_outputs(pico_fpga_timing)

// =============================
// File: src/protocol.h
// =============================
#pragma once
#include <stdint.h>
#include <stdbool.h>

// --------------------- PROTOCOL (from repo README) ---------------------
// First byte (op):
// Bit7: 0=BRAM, 1=SPRAM
// Bit6: 0=READ, 1=WRITE
// Bit5: 1=Warmboot, 0=Normal memory op
// Bits4:0: Block select (for warmboot: bits1:0 = image number)
//
// BRAM op stream:   [OP][ADDR8][SIZE8=(N-1)][DATA.. 2*N bytes if WRITE]
// SPRAM op stream:  [OP][ADDR16_H][ADDR16_L][SIZE8=(N-1)][DATA.. 2*N bytes if WRITE]
// For READ, FPGA returns 2*N bytes (word order as transmitted). We assume LE word order on the wire (low byte then high byte) unless your RTL uses BE—adjust if needed.

// Helpers to build op byte
static inline uint8_t op_byte(bool spram, bool write, uint8_t block_or_img, bool warmboot){
    uint8_t op = 0;
    if (spram) op |= 0x80;        // bit7
    if (write) op |= 0x40;        // bit6
    if (warmboot) op |= 0x20;     // bit5
    op |= (block_or_img & 0x1F);  // bits4:0
    return op;
}

// =============================
// File: src/transport.h
// =============================
#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t elapsed_us;   // host-side measured µs
    int rc;                // 0 ok, <0 error
} xfer_result_t;

void link_init(uint32_t baud, int pin_tx, int pin_rx);

// BRAM (16-bit words)
xfer_result_t bram_write(uint8_t block, uint8_t addr8, const uint16_t *words, uint16_t n_words);
xfer_result_t bram_read (uint8_t block, uint8_t addr8,       uint16_t *words, uint16_t n_words);

// SPRAM (16-bit words)
xfer_result_t spram_write(uint8_t block, uint16_t addr16, const uint16_t *words, uint16_t n_words);
xfer_result_t spram_read (uint8_t block, uint16_t addr16,       uint16_t *words, uint16_t n_words);

// Warmboot (image 0..3)
xfer_result_t warmboot(uint8_t image_sel);

#ifdef __cplusplus
}
#endif

// =============================
// File: src/util.h
// =============================
#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

uint32_t now_us(void);
void rng_seed(uint32_t s);
uint32_t rng_u32(void);
uint16_t addr_seq(size_t i, uint16_t lo, uint16_t hi);

void csv_header(void);
void csv_row(const char *test, const char *op, uint32_t addr, uint32_t data,
             uint32_t us, int rc);

// =============================
// File: src/util.c
// =============================
#include "util.h"
#include <stdio.h>
#include "pico/stdlib.h"

uint32_t now_us(void){ return time_us_32(); }

static uint32_t s_lcg = 1;
void rng_seed(uint32_t s){ s_lcg = s ? s : 1; }
uint32_t rng_u32(void){ s_lcg = 1664525u * s_lcg + 1013904223u; return s_lcg; }

uint16_t addr_seq(size_t i, uint16_t lo, uint16_t hi){ (void)i; uint32_t span = (uint32_t)hi - (uint32_t)lo + 1; return (uint16_t)(lo + (rng_u32() % span)); }

void csv_header(void){ printf("test,op,addr,data,elapsed_us,rc
"); }
void csv_row(const char *test, const char *op, uint32_t addr, uint32_t data, uint32_t us, int rc){
    printf("%s,%s,0x%04lX,0x%08lX,%lu,%d
", test, op, (unsigned long)addr, (unsigned long)data, (unsigned long)us, rc);
}

// =============================
// File: src/transport_uart.c
// =============================
#include "transport.h"
#include "protocol.h"
#include "util.h"
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#ifndef UART1_BAUD
#define UART1_BAUD 2000000
#endif
#ifndef PIN_UART1_TX
#define PIN_UART1_TX 8
#endif
#ifndef PIN_UART1_RX
#define PIN_UART1_RX 9
#endif

#define UART_PORT uart1

static inline void put_u16_le(uint8_t *p, uint16_t v){ p[0]=(uint8_t)v; p[1]=(uint8_t)(v>>8); }
static inline uint16_t get_u16_le(const uint8_t *p){ return (uint16_t)p[0] | (uint16_t)p[1]<<8; }

void link_init(uint32_t baud, int pin_tx, int pin_rx){
    uart_init(UART_PORT, baud ? baud : UART1_BAUD);
    gpio_set_function(pin_tx>0?pin_tx:PIN_UART1_TX, GPIO_FUNC_UART);
    gpio_set_function(pin_rx>0?pin_rx:PIN_UART1_RX, GPIO_FUNC_UART);
    uart_set_format(UART_PORT, 8, 1, UART_PARITY_NONE);
}

static inline void uwrite(const uint8_t *b, size_t n){ for(size_t i=0;i<n;++i) uart_putc_raw(UART_PORT, b[i]); }
static inline void uread(uint8_t *b, size_t n){ for(size_t i=0;i<n;++i) b[i]=(uint8_t)uart_getc(UART_PORT); }

static xfer_result_t do_bram(bool write, uint8_t block, uint8_t addr8, uint16_t *words, uint16_t n_words){
    xfer_result_t xr={0}; if(n_words==0||n_words>256) { xr.rc=-1; return xr; }
    uint8_t op = op_byte(false, write, block & 0x1F, false);
    uint8_t size = (uint8_t)(n_words - 1);

    uint32_t t0 = now_us();
    uwrite(&op,1);
    uwrite(&addr8,1);
    uwrite(&size,1);
    if(write){
        for(uint16_t i=0;i<n_words;++i){ uint8_t b[2]; put_u16_le(b, words[i]); uwrite(b,2);}    
    } else {
        for(uint16_t i=0;i<n_words;++i){ uint8_t b[2]; uread(b,2); words[i]=get_u16_le(b);} 
    }
    uint32_t t1 = now_us(); xr.elapsed_us=t1-t0; xr.rc=0; return xr;
}

static xfer_result_t do_spram(bool write, uint8_t block, uint16_t addr16, uint16_t *words, uint16_t n_words){
    xfer_result_t xr={0}; if(n_words==0||n_words>256) { xr.rc=-1; return xr; }
    uint8_t op = op_byte(true, write, block & 0x1F, false);
    uint8_t addr_b[2]; put_u16_le(addr_b, addr16);
    uint8_t size = (uint8_t)(n_words - 1);

    uint32_t t0 = now_us();
    uwrite(&op,1);
    uwrite(addr_b,2);
    uwrite(&size,1);
    if(write){
        for(uint16_t i=0;i<n_words;++i){ uint8_t b[2]; put_u16_le(b, words[i]); uwrite(b,2);}    
    } else {
        for(uint16_t i=0;i<n_words;++i){ uint8_t b[2]; uread(b,2); words[i]=get_u16_le(b);} 
    }
    uint32_t t1 = now_us(); xr.elapsed_us=t1-t0; xr.rc=0; return xr;
}

xfer_result_t bram_write(uint8_t block, uint8_t addr8, const uint16_t *words, uint16_t n_words){ return do_bram(true,  block, addr8, (uint16_t*)words, n_words); }
xfer_result_t bram_read (uint8_t block, uint8_t addr8,       uint16_t *words, uint16_t n_words){ return do_bram(false, block, addr8, words, n_words); }

xfer_result_t spram_write(uint8_t block, uint16_t addr16, const uint16_t *words, uint16_t n_words){ return do_spram(true,  block, addr16, (uint16_t*)words, n_words); }
xfer_result_t spram_read (uint8_t block, uint16_t addr16,       uint16_t *words, uint16_t n_words){ return do_spram(false, block, addr16, words, n_words); }

xfer_result_t warmboot(uint8_t image_sel){
    xfer_result_t xr={0}; uint8_t op = op_byte(false,false, image_sel & 0x03, true);
    uint32_t t0=now_us(); uwrite(&op,1); uint32_t t1=now_us(); xr.elapsed_us=t1-t0; xr.rc=0; return xr;
}

// =============================
// File: src/main.c
// =============================
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "transport.h"
#include "util.h"

#ifndef UART_LOG_BAUD
#define UART_LOG_BAUD 115200
#endif
#ifndef UART1_BAUD
#define UART1_BAUD 2000000
#endif
#ifndef PIN_UART1_TX
#define PIN_UART1_TX 8
#endif
#ifndef PIN_UART1_RX
#define PIN_UART1_RX 9
#endif

static void print_help(void){
    printf("
Commands (repo-compatible):
");
    printf("  help
");
    printf("  link <baud>                 (UART1 baud to FPGA)
");
    printf("  bram wr <blk> <addr8> <n>   (then sends n 16-bit words 0..n-1)
");
    printf("  bram rd <blk> <addr8> <n>
");
    printf("  spram wr <blk> <addr16> <n> (then sends n 16-bit words 0..n-1)
");
    printf("  spram rd <blk> <addr16> <n>
");
    printf("  warmboot <image 0..3>
");
    printf("  bench fixed <mem> <blk> <addr> <n> <iters>   (mem=bram|spram)
");
    printf("  bench span  <mem> <blk> <lo> <hi> <n> <iters>
");
}

static void dump_words(const uint16_t *w, uint16_t n){ for(uint16_t i=0;i<n;++i){ printf(i?" %04X":"%04X", w[i]); } printf("
"); }

int main(){
    stdio_init_all(); sleep_ms(300);
    printf("
[Pico] pico_fpga_timing (repo protocol) starting...
");
    link_init(UART1_BAUD, PIN_UART1_TX, PIN_UART1_RX);
    print_help();

    char line[160];
    while(1){
        printf("
> "); fflush(stdout);
        // simple blocking line read
        size_t i=0; while(i+1<sizeof line){ int c=getchar_timeout_us(0); if(c==PICO_ERROR_TIMEOUT) continue; if(c=='') continue; if(c=='
'){ line[i]=' '; break; } line[i++]=(char)c; putchar(c);} if(!i) continue;

        if(!strcmp(line,"help")){ print_help(); continue; }
        if(!strncmp(line,"link ",5)){ uint32_t b=strtoul(line+5,NULL,10); link_init(b, PIN_UART1_TX, PIN_UART1_RX); printf("ok baud=%u
", b); continue; }

        // BRAM/SPRAM single ops
        if(!strncmp(line,"bram wr ",8)){
            unsigned blk, addr, n; if(sscanf(line+8, "%u %x %u", &blk, &addr, &n)==3){
                if(n==0||n>256){ printf("err n 1..256
"); continue; }
                uint16_t *buf=(uint16_t*)malloc(n*2); for(unsigned k=0;k<n;++k) buf[k]=(uint16_t)k;
                xfer_result_t xr=bram_write((uint8_t)blk,(uint8_t)addr,buf,(uint16_t)n);
                printf("ok us=%lu rc=%d
", (unsigned long)xr.elapsed_us, xr.rc); free(buf);
            } continue; }
        if(!strncmp(line,"bram rd ",8)){
            unsigned blk, addr, n; if(sscanf(line+8, "%u %x %u", &blk, &addr, &n)==3){
                uint16_t *buf=(uint16_t*)malloc(n*2);
                xfer_result_t xr=bram_read((uint8_t)blk,(uint8_t)addr,buf,(uint16_t)n);
                printf("ok us=%lu rc=%d data=", (unsigned long)xr.elapsed_us, xr.rc); dump_words(buf,(uint16_t)n); free(buf);
            } continue; }
        if(!strncmp(line,"spram wr ",9)){
            unsigned blk, addr, n; if(sscanf(line+9, "%u %x %u", &blk, &addr, &n)==3){
                if(n==0||n>256){ printf("err n 1..256
"); continue; }
                uint16_t *buf=(uint16_t*)malloc(n*2); for(unsigned k=0;k<n;++k) buf[k]=(uint16_t)k;
                xfer_result_t xr=spram_write((uint8_t)blk,(uint16_t)addr,buf,(uint16_t)n);
                printf("ok us=%lu rc=%d
", (unsigned long)xr.elapsed_us, xr.rc); free(buf);
            } continue; }
        if(!strncmp(line,"spram rd ",9)){
            unsigned blk, addr, n; if(sscanf(line+9, "%u %x %u", &blk, &addr, &n)==3){
                uint16_t *buf=(uint16_t*)malloc(n*2);
                xfer_result_t xr=spram_read((uint8_t)blk,(uint16_t)addr,buf,(uint16_t)n);
                printf("ok us=%lu rc=%d data=", (unsigned long)xr.elapsed_us, xr.rc); dump_words(buf,(uint16_t)n); free(buf);
            } continue; }

        if(!strncmp(line,"warmboot ",9)){
            unsigned img; if(sscanf(line+9, "%u", &img)==1){ xfer_result_t xr=warmboot((uint8_t)img); printf("ok warmboot us=%lu rc=%d
", (unsigned long)xr.elapsed_us, xr.rc);} continue; }

        // Benchmarks that produce CSV
        if(!strncmp(line,"bench fixed ",12)){
            char mem[8]; unsigned blk, addr, n, it; if(sscanf(line+12, "%7s %u %x %u %u", mem, &blk, &addr, &n, &it)==5){
                int is_bram = !strcmp(mem,"bram"); csv_header();
                for(unsigned i2=0;i2<it;++i2){
                    uint16_t *buf=(uint16_t*)malloc(n*2); for(unsigned k=0;k<n;++k) buf[k]=(uint16_t)(k+i2);
                    xfer_result_t wr = is_bram? bram_write((uint8_t)blk,(uint8_t)addr,buf,(uint16_t)n)
                                              : spram_write((uint8_t)blk,(uint16_t)addr,buf,(uint16_t)n);
                    csv_row("FIXED","WRITE", addr, n, wr.elapsed_us, wr.rc);
                    xfer_result_t rd = is_bram? bram_read((uint8_t)blk,(uint8_t)addr,buf,(uint16_t)n)
                                              : spram_read((uint8_t)blk,(uint16_t)addr,buf,(uint16_t)n);
                    csv_row("FIXED","READ",  addr, n, rd.elapsed_us, rd.rc);
                    free(buf);
                }
            } continue; }
        if(!strncmp(line,"bench span ",11)){
            char mem[8]; unsigned blk, lo, hi, n, it; if(sscanf(line+11, "%7s %u %x %x %u %u", mem, &blk, &lo, &hi, &n, &it)==6){
                int is_bram = !strcmp(mem,"bram"); csv_header(); rng_seed(123);
                for(unsigned i2=0;i2<it;++i2){
                    uint16_t addr = (uint16_t)addr_seq(i2, (uint16_t)lo, (uint16_t)hi);
                    uint16_t *buf=(uint16_t*)malloc(n*2); for(unsigned k=0;k<n;++k) buf[k]=(uint16_t)rng_u32();
                    xfer_result_t wr = is_bram? bram_write((uint8_t)blk,(uint8_t)addr,buf,(uint16_t)n)
                                              : spram_write((uint8_t)blk,(uint16_t)addr,buf,(uint16_t)n);
                    csv_row("SPAN","WRITE", addr, n, wr.elapsed_us, wr.rc);
                    xfer_result_t rd = is_bram? bram_read((uint8_t)blk,(uint8_t)addr,buf,(uint16_t)n)
                                              : spram_read((uint8_t)blk,(uint16_t)addr,buf,(uint16_t)n);
                    csv_row("SPAN","READ",  addr, n, rd.elapsed_us, rd.rc);
                    free(buf);
                }
            } continue; }

        printf("? unknown command. type 'help'
");
    }
}

// =============================
// USAGE NOTES (repo alignment)
// =============================
// • UART1 wires to the FPGA's uart_controller. No start/end markers are used.
// • Words transmitted little-endian (low byte first). If your Python host uses
//   big-endian packing, flip get/put functions above.
// • Size byte is (N-1) per README. N range is 1..256.
// • BRAM blocks: hx1k=16; up5k=30. SPRAM blocks (up5k): 4. Configure block index accordingly.
// • warmboot() just emits the op byte with bit5=1 and bits1:0=image.
// • The CLI includes minimal commands and two benchmark drivers that stream CSV.
