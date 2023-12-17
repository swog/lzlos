//====================================
// Keyboard interrupt handler
//
//====================================

#ifndef _KBD_H
#define _KBD_H

#pragma once

struct kisrcall_s;

int kbd_irq(struct kisrcall_s *info);
void kbd_main();

#endif
