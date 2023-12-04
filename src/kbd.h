//====================================
// Keyboard interrupt handler
//
//====================================

#ifndef _KBD_H
#define _KBD_H

#pragma once

struct kisrcall_s;

void kbd_irq(struct kisrcall_s *info);

#endif
