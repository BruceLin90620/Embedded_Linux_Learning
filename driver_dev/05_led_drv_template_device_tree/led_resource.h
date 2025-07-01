#ifndef _LED_RESOURCE_H
#define _LED_RESOURCE_H

#define GROUP(x) (x>>16)
#define PIN(x)   (x&0xFFFF)
#define GROUP_PIN(g,p) ((g<<16) | (p))

#endif