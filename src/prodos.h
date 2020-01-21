#ifndef APOEKDMFOS_PRODOS_H
#define APOEKDMFOS_PRODOS_H


/* Maximum partial pathname for ProDOS is 64 bytes without a nul
   terminator but including slashes. 
   ProDOS automatically adds the Prefix, which can also be 64 bytes, for 
   a maximum length of 128 bytes. As you can see, this will be 
   problematic. The only way to get to the full 128 bytes is to set
   the Prefix because you can't specify 128 bytes in a pathname, but
   only 64. Fun? */
#define PRODOS_MAX_PATH_LENGTH 64
#define PRODOS_PATH_SIZE (PRODOS_MAX_PATH_LENGTH+1)

#endif

