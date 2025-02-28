#ifndef _TUD_HANDLER_H
#define _TUD_HANDLER_H


#include <inttypes.h>
#include <stdbool.h>
#include "tusb.h"



void tud_mount_cb(void);
void tud_umount_cb(void);
void tud_suspend_cb(bool remote_wakeup_en);
void tud_resume_cb(void);


#endif
