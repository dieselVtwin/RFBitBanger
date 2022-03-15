/* ui.h

*/

/*
   Copyright (c) 2021 Daniel Marks

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

#ifndef _UI_H
#define _UI_H

typedef struct _menu_str
{
  const char * const* items;
  uint8_t col;
  uint8_t row;
  uint8_t width;
  uint8_t item;
} menu_str;


void set_horiz_menu_keys(uint8_t horiz);
uint8_t do_menu(menu_str *menu);
void do_show_menu_item(menu_str *menu);
bool show_messages(const char *message1, const char *message2);
bool go_or_abort(const char *message);
char *number_str(char *s, uint32_t n, uint8_t digits, uint8_t decs);
void lcdPrint(const char *str);
void lcdPrintFlash(const char *str);
void lcdPrintNum(uint32_t n, uint8_t digits, uint8_t decs);

typedef struct _bargraph_dat
{
  uint8_t num_bars;
  uint8_t width_bars;
  uint8_t col_bars;
  uint8_t row_bars;
  uint8_t bars[4];
} bargraph_dat;

typedef struct _scroll_number_dat
{
  uint8_t     col;
  uint8_t     row;
  uint8_t     digits;
  uint8_t     decs;
  uint32_t    minimum_number;
  uint32_t    maximum_number;

  uint8_t     position;
  uint32_t    n;
  uint8_t     entered;
  uint8_t     changed;
} scroll_number_dat;

typedef struct _scroll_alpha_dat
{
  uint8_t        col;
  uint8_t        row;
  uint8_t        displen;
  uint8_t        numchars;
  uint8_t       *buffer;
  const uint8_t *validchars;
  uint8_t        num_validchars;
  uint8_t        position;
  uint8_t        cursorpos;
  uint8_t        changed;
  uint8_t        entered;
} scroll_alpha_dat;

void scroll_number_stop(scroll_number_dat *snd);
void scroll_number_start(scroll_number_dat *snd);
void scroll_number_key(scroll_number_dat *snd);
void scroll_number_redraw(scroll_number_dat *snd);

void scroll_alpha_stop(scroll_alpha_dat *sad);
void scroll_alpha_start(scroll_alpha_dat *sad);
void scroll_alpha_key(scroll_alpha_dat *sad);
void scroll_alpha_redraw(scroll_alpha_dat *sad);

void lcdBarGraph(bargraph_dat *bgd);
uint8_t abort_button_left(void);
uint8_t abort_button_right(void);

#endif  /* _UI_H */
