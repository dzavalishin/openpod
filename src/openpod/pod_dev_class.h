#ifndef POD_DEV_CLASS_H
#define POD_DEV_CLASS_H

//*******************************************************************
//
// OpenPOD
//
// Device (or driver) classes.
//
// Device class defines a set of operations available for it.
//
//*******************************************************************


#define POD_DEV_CLASS_VOID	0 // undefined or none
#define POD_DEV_CLASS_SPECIAL	1 // has user-defined non-standard interface
#define POD_DEV_CLASS_VIDEO	2 // framebuf, bitblt io
#define POD_DEV_CLASS_BLOCK	3 // disk, cdrom - block io
#define POD_DEV_CLASS_CHARACTER	4 // tty, serial, byte io
#define POD_DEV_CLASS_NET	5 // packet IO, has MAC address

#define POD_DEV_CLASS_KEYBD	6 // key (make/break) events
#define POD_DEV_CLASS_MOUSE	7 // mouse coordinate events

#define POD_DEV_CLASS_MULTIPLE	0xFF // driver only, has multiple dev types



#eneif // POD_DEV_CLASS_H
