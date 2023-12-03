
/*  dspint.h */

/*
 * Copyright (c) 2021 Daniel Marks

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
 */

#ifndef __DSPINT_H
#define __DSPINT_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _dsp_txmit_message_state
{ 
  void     *user_state;
  uint32_t  frequency;
  uint8_t  *message;
  uint8_t   length;
  uint8_t   current_symbol;
  uint8_t   aborted;
} dsp_txmit_message_state;

typedef void (*dsp_dispatch_callback)(struct _dsp_txmit_message_state *);

#define SCAMP_PROTOCOL
#define CW_PROTOCOL
#define RTTY_PROTOCOL
#ifdef SSB_SUPPORT
#define SSB_PROTOCOL
#endif

#ifdef SCAMP_PROTOCOL
#include "scamp.h"
#endif /* SCAMP_PROTOCOL */
#ifdef CW_PROTOCOL
#include "cwmod.h"
#endif /* CW_PROTOCOL */
#ifdef RTTY_PROTOCOL
#include "rtty.h"
#endif /* RTTY_PROTOCOL */
#ifdef SSB_PROTOCOL
#include "ssb.h"
#endif /* SSB_PROTOCOL */

#ifdef SCAMP_VERY_SLOW_MODES
#define DSPINT_MAX_SAMPLEBUFFER 144
#else
#define DSPINT_MAX_SAMPLEBUFFER 64
#endif

#define PROTOCOL_FASTSCAN       0 
#define PROTOCOL_CW             1
#define PROTOCOL_USB            2
#define PROTOCOL_LSB            3 
#define PROTOCOL_RTTY           4
#define PROTOCOL_RTTY_REV       5
#define PROTOCOL_SCAMP_FSK      6
#define PROTOCOL_SCAMP_OOK      7
#define PROTOCOL_SCAMP_FSK_FAST 8
#ifdef SCAMP_VERY_SLOW_MODES
#define PROTOCOL_SCAMP_FSK_SLOW 9
#define PROTOCOL_SCAMP_OOK_SLOW 10
#define PROTOCOL_SCAMP_FSK_VSLW 11
#define PROTOCOL_SCAMP_LAST_MODE 11
#else
#define PROTOCOL_SCAMP_LAST_MODE 8
#endif

#define IS_SSB_PROTOCOL(x) (((x) == PROTOCOL_USB) || ((x) == PROTOCOL_LSB))
#define IS_SCAMP_PROTOCOL(x) (((x) >= PROTOCOL_SCAMP_FSK) && ((x) <= PROTOCOL_SCAMP_LAST_MODE))

typedef void (*dsp_interrupt_routine)(void);
typedef void (*dsp_decode_process)(void);
typedef uint8_t (*dsp_xmit_routine)(dsp_txmit_message_state *, dsp_dispatch_callback);
typedef int16_t (*dsp_frequency_offset)(void);

/* this is stuff that is initialized when the modulation mode
   is changed and doesn't change otherwise */
typedef struct _dsp_state_fixed
{
  uint8_t   buffer_size;
  uint8_t   dly_8;
  uint8_t   dly_12;
  uint8_t   dly_16;
  uint8_t   dly_20;
  uint8_t   dly_24;
  uint8_t   slow_samp_num;

  dsp_interrupt_routine dir;
  dsp_decode_process ddp;
  dsp_xmit_routine dxr;
  dsp_frequency_offset dfo;
} dsp_state_fixed;

/* this is the current state of the demodulator and is designed
   so that is might be reset quickly without disturbing the
   dsp_state_fixed state */
typedef struct _dsp_state
{
  volatile uint8_t   ssb_active;

  uint8_t   slow_samp;
  uint16_t  total_num;

  uint8_t   sample_no;
  uint8_t   sample_ct;

  uint16_t  ct_average;
  uint32_t  ct_sum;

  uint8_t   count;
  uint8_t   count_8;
  uint8_t   count_12;
  uint8_t   count_16;
  uint8_t   count_20;
  uint8_t   count_24;

  int32_t  state_i_8;
  int32_t  state_i_12;
  int32_t  state_i_16;
  int32_t  state_i_20;
  int32_t  state_i_24;

  int32_t  state_q_8;
  int32_t  state_q_12;
  int32_t  state_q_16;
  int32_t  state_q_20;
  int32_t  state_q_24;

  uint16_t  mag_value_8;
  uint16_t  mag_value_12;
  uint16_t  mag_value_16;
  uint16_t  mag_value_20;
  uint16_t  mag_value_24;

  uint16_t  sample_buffer[DSPINT_MAX_SAMPLEBUFFER];

} dsp_state;

extern dsp_state       ds;
extern dsp_state_fixed df;

typedef struct _null_state
{
  uint8_t   protocol;
} null_state;

typedef union _protocol_state
{
    null_state   ns;
#ifdef SCAMP_PROTOCOL
    scamp_state  ss;
#endif
#ifdef CW_PROTOCOL
    cwmod_state  cs;
#endif
#ifdef RTTY_PROTOCOL
    rtty_state   rs;
#endif
#ifdef SSB_PROTOCOL
    ssb_state    ssbs;
#endif
} protocol_state;

extern protocol_state  ps;

void dsp_interrupt_sample(uint16_t sample);
void dsp_new_sample(void);
void dsp_initialize_scamp(uint8_t protocol, uint8_t wide);
void dsp_initialize_cw(uint8_t protocol, uint8_t wide);
void dsp_initialize_rtty(uint8_t protocol, uint8_t wide);
void dsp_initialize_fastscan(void);
void dsp_reset_state(void);
uint16_t dsp_get_signal_magnitude(void);
uint8_t dsp_dispatch_txmit(uint32_t frequency, uint8_t *message, uint8_t length, void *user_state, dsp_dispatch_callback ddc);
void dsp_dispatch_receive(void);
void dsp_dispatch_interrupt(void);
void dsp_initialize_protocol(uint8_t protocol, uint8_t wide);
int16_t dsp_freq_offset(void);

#define DECODE_FIFO_LENGTH 16

/* structure for decode fifo */
typedef struct _decode_fifo
{
    uint8_t   head;
    uint8_t   tail;
    uint8_t   decodes[DECODE_FIFO_LENGTH];
} decode_fifo;

void decode_initialize_fifo(void);
uint8_t decode_insert_into_fifo(uint8_t c);
uint16_t decode_remove_from_fifo(void);

#ifdef __cplusplus
}
#endif

#endif  /* __DSPINT_H */
