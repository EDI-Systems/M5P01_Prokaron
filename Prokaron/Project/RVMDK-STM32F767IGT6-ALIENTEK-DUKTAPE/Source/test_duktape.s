Heap_Size       EQU             65536*4
                
                AREA            HEAP, NOINIT, READWRITE, ALIGN=3
                ALIGN
__heap_base
Heap_Mem        SPACE           Heap_Size
__heap_limit

                ;The align is "(2^3)/8=1(Byte)." In fact it does not take effect.            
                AREA            TEXT,CODE,READONLY,ALIGN=3
                ALIGN
                IF              :DEF:__MICROLIB
                EXPORT          __heap_base
                EXPORT          __heap_limit
                ELSE
                IMPORT          __use_two_region_memory
                EXPORT          __user_initial_stackheap
__user_initial_stackheap
                LDR             R0,=Heap_Mem
                LDR             R1,=(Stack_Mem+Stack_Size)
                LDR             R2,=(Heap_Mem+Heap_Size)
                LDR             R3,=Stack_Mem
                BX              LR
                ENDIF
                END
