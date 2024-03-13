#include "idt.h"

void init_idt() {
    int i;

    for (i = 0; i < NUM_VEC; i++) {
        // set present bit
        if ((i < NUM_EXCEPTIONS && i != RESTRICTED_EXCEPTION) || i == SYSTEM_CALLS || i == KEYBOARD_CALLS || i == RTC_CALLS) {
            idt[i].present = 1;
        } else {
            idt[i].present = 0;
        }

        // set dpl to 0 for exceptions and 3 for system calls
        if (i == SYSTEM_CALLS) {
            idt[i].dpl = 3;
        } else {
            idt[i].dpl = 0;
        }

        // set reserved0 to 0
        idt[i].reserved0 = 0;

        // set gate type (reserved3 is 1 for keyboard and rtc)
        idt[i].size = 1;
        idt[i].reserved1 = 1;
        idt[i].reserved2 = 1;
        if (i == KEYBOARD_CALLS || i == RTC_CALLS) {
            idt[i].reserved3 = 1;
        } else {
            idt[i].reserved3 = 0;
        }

        // set reserved4 to 0
        idt[i].reserved4 = 0;
        
        // set seg selector to kernel code segment
        idt[i].seg_selector = KERNEL_CS;
    }

    
    SET_IDT_ENTRY(idt[0], division_error); // Division Error
    SET_IDT_ENTRY(idt[1], debug); // Debug
    SET_IDT_ENTRY(idt[2], non_maskable_interrupt); // Non-maskable Interrupt
    SET_IDT_ENTRY(idt[3], breakpoint); // Breakpoint
    SET_IDT_ENTRY(idt[4], overflow); // Overflow
    SET_IDT_ENTRY(idt[5], bound_range_exceeded); // Bound Range Exceeded
    SET_IDT_ENTRY(idt[6], invalid_opcode); // Invalid Opcode
    SET_IDT_ENTRY(idt[7], device_not_available); // Device Not Available
    SET_IDT_ENTRY(idt[8], double_fault); // Double Fault
    SET_IDT_ENTRY(idt[9], coprocessor_segment_overrun); // Coprocessor Segment Overrun
    SET_IDT_ENTRY(idt[10], invalid_tss); // Invalid TSS
    SET_IDT_ENTRY(idt[11], segment_not_present); // Segment Not Present
    SET_IDT_ENTRY(idt[12], stack_segment_fault); // Stack-Segment Fault
    SET_IDT_ENTRY(idt[13], general_protection_fault); // General Protection Fault
    SET_IDT_ENTRY(idt[14], page_fault); // Page Fault
    SET_IDT_ENTRY(idt[16], asm_floating_point_exception); // x87 Floating-Point Exception
    SET_IDT_ENTRY(idt[17], alignment_check); // Alignment Check
    SET_IDT_ENTRY(idt[18], machine_check); // Machine Check
    SET_IDT_ENTRY(idt[19], simd_floating_point_exception); // SIMD Floating-Point Exception
    SET_IDT_ENTRY(idt[SYSTEM_CALLS], system_call); // System Calls
<<<<<<< HEAD
    SET_IDT_ENTRY(idt[KEYBOARD_CALLS], system_call); // Keyboard Calls
    SET_IDT_ENTRY(idt[RTC_CALLS], system_call); // RTC Calls

    lidt(idt_desc_ptr); // load the idt
=======
    SET_IDT_ENTRY(idt[KEYBOARD_CALLS], keyboard_call); // Keyboard Calls
    SET_IDT_ENTRY(idt[RTC_CALLS], rtc_call); // RTC Calls

    lidt(idt_desc_ptr); //load the idt
>>>>>>> c9e13e803f3fa8c639eaf6b9f81ffc117a79e2fa
}
