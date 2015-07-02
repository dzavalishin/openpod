#include "pod_bochs.h"
#include "pod_bochs_vbe.h"


void pod_bocsh_vbe_write(unsigned short index, unsigned short value)
{
   outw(VBE_DISPI_IOPORT_INDEX, index);
   outw(VBE_DISPI_IOPORT_DATA, value);
}

int pod_bocsh_vbe_read(unsigned short index)
{
    outw(VBE_DISPI_IOPORT_INDEX, index);
    return inw(VBE_DISPI_IOPORT_DATA);
}

