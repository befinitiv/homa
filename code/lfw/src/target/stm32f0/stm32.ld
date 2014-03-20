MEMORY
{
	rom (rx)     : ORIGIN = 0x08000000, LENGTH = 64K
	ram (rwx)    : ORIGIN = 0x20000000, LENGTH = 8K
	garbage (rw) : ORIGIN = 0x00000000, LENGTH = 1K
}

/* Enforce emmition of the vector table. */
EXTERN (vector_table)

ENTRY(reset_handler)

EXTERN(moduleInitializeFunction0)
EXTERN(moduleInitializeFunction1)
EXTERN(moduleInitializeFunction2)
EXTERN(moduleInitializeFunction3)
EXTERN(moduleInitializeFunction4)
EXTERN(moduleInitializeFunction5)
EXTERN(moduleInitializeFunction6)
EXTERN(moduleInitializeFunction7)
EXTERN(moduleInitializeFunction8)
EXTERN(moduleInitializeFunction9)

EXTERN(genericProtocolEndpointHandles)
EXTERN(genericProtocolEndpointInternalHandles)

EXTERN(logStrings)

EXTERN(msgPumpDescriptions)

/* Define sections. */
SECTIONS
{
        . = ORIGIN(rom);

        .text : {
                *(.vectors)     /* Vector table */
                *(.text*)       /* Program code */
                . = ALIGN(4);
                *(.rodata*)     /* Read-only data */
                _initFunctions_Begin = .;
                KEEP(*(.moduleInitializeFunction0*))
                KEEP(*(.moduleInitializeFunction1*))
                KEEP(*(.moduleInitializeFunction2*))
                KEEP(*(.moduleInitializeFunction3*))
                KEEP(*(.moduleInitializeFunction4*))
                KEEP(*(.moduleInitializeFunction5*))
                KEEP(*(.moduleInitializeFunction6*))
                KEEP(*(.moduleInitializeFunction7*))
                KEEP(*(.moduleInitializeFunction8*))
                KEEP(*(.moduleInitializeFunction9*))
                _initFunctions_End = .;
                
	            _logStringsBegin = .;
	            KEEP(*(.logStrings*))
	            _logStringsEnd = .;
	            
				KEEP(*(.genericProtocolEndpointHandles*))
				_genericProtocolEndpointInternalHandlesBegin = .;
				KEEP(*(.genericProtocolEndpointInternalHandles*))
				_genericProtocolEndpointInternalHandlesEnd = .;
				
                _msgPumpBPHandlesBegin = .;
                KEEP(*(.msgPumpDescriptions*))
                _msgPumpBPHandlesEnd = .;
                
                _etext = .;
        } >rom

        . = ORIGIN(ram);

        .data : {
                _data = .;
                *(.data*)       /* Read-write initialized data */
                . = ALIGN(4);
                _edata = .;
        } >ram AT > rom
        PROVIDE(_data_loadaddr = LOADADDR(.data));

        .bss : {
    			. = ALIGN(4);
    			_bss = .;
                *(.bss*)        /* Read-write zero initialized data */
                *(COMMON)
                . = ALIGN(4);
                _ebss = .;
        } >ram
		
        . = ALIGN(4);
        end = .;
        
        .garbage : AT(ORIGIN(garbage)) 
        {
        	*(.*NEVER*)
        } >garbage
}

PROVIDE(_dataAtFlash = LOADADDR(.data));

PROVIDE(_stack = ORIGIN(ram) + LENGTH(ram));
PROVIDE(_ramBegin = ORIGIN(ram));
