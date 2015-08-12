#include <stdio.h>
#include <conio.h>  /* clrscr */
#include <6502.h>   /* getcpu */

#define CPU_6502_NAME "6502"
#define CPU_65C02_NAME "65C02"
#define CPU_65816_NAME "65816"
#define CPU_UNKNOWN "Unknown"

static char *get_cpu_name();
static char *get_machine_hardware_name();

void main()
{
    clrscr();
    cputs(get_machine_hardware_name());
    cputc(' ');
    cputs(get_cpu_name());
    cputs("\r\n");

    getchar();
}

char *get_cpu_name()
{
    switch (getcpu()) {
        case CPU_6502:
            return CPU_6502_NAME;
        case CPU_65C02:
            return CPU_65C02_NAME;
        case CPU_65816:
            return CPU_65816_NAME;
        default:
            return CPU_UNKNOWN;
    }
}

char *get_machine_hardware_name()
{
    switch (get_ostype()) {
        case APPLE_UNKNOWN: return "Unknown";
        case APPLE_II:      return "Apple ][";
        case APPLE_IIPLUS:  return "Apple ][+";
        case APPLE_IIIEM:   return "Apple /// (emulation)";
        case APPLE_IIE:     return "Apple //e";
        case APPLE_IIEENH:  return "Apple //e (enhanced)";
        case APPLE_IIECARD: return "Apple //e Option Card";
        case APPLE_IIC:     return "Apple //c";
        case APPLE_IIC35:   return "Apple //c (3.5 ROM)";
        case APPLE_IICEXP:  return "Apple //c (Mem. Exp.)";
        case APPLE_IICREV:  return "Apple //c (Rev. Mem. Exp.)";
        case APPLE_IICPLUS: return "Apple //c Plus";
        case APPLE_IIGS:    return "Apple IIgs";
        case APPLE_IIGS1:   return "Apple IIgs (ROM 1)";
        case APPLE_IIGS3:   return "Apple IIgs (ROM 3)";
        default:            return "Unknown";
    }
}

