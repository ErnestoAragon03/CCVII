# ARM Cortex-A8 Characteristics
## 2 Programmers Model
### 2.13 Registers
- 3 processor operating states
    - - ARM State
    - - Thumb State
    - - ThumbEE State
- 8 modes of operation
    - - User Mode
    - - Fast Interrupt (FIQ)
    - - Interupt (IRQ)
    - - Supervisor mode
    - - Abort Mode
    - - System Mode
    - - Undefined Mode
    - - Monitor Mode

- Registers:
    - - 33 general-purpose 32-bit registers
    - - 7 32-bit status registers
These registers are not all accessible at the same time. The processor state and mode of operation determine which registers are available to the programmer.

- r14 is the Link Register
- r15 is the Program Counter

- Banked registers have a mode identifier that indicates the mode that they relate to. 

| Mode           | Identifier |
|:--------------:|:----------:|
| User           | usr        |
| Fast Interrupt | fiq        |
| Interrupt      | irq        |
| Supervisor     | svc        |
| Abort          | abt        |
| System         | usr        |
| Undefined      | und        |
| Monitor        | mon        |

FIQ mode has seven banked registers mapped to r8–r14, that is, r8_fiq through r14_fiq.
As a result many FIQ handlers do not have to save any registers

The Monitor, Supervisor, Abort, IRQ, and Undefined modes have alternative
mode-specific registers mapped to r13 and r14, that permits a private stack pointer and
link register for each mode.

### The program Status Registers
The processor contains one CPSR, and six SPSRs for exception handlers to use. The
program status registers:
• hold information about the most recently performed logical or arithmetic
operation
• control the enabling and disabling of interrupts
• set the processor operating mode.

(See Figure 2-12 Program Status Register)

### Exceptions
Exceptions occur whenever the processor temporarily halts the normal flow of a
program, for example, to service an interrupt from a peripheral. Before attempting to
handle an exception, the processor preserves the current processor state so the original
program can resume when the handler routine finishes.
If two or more exceptions occur simultaneously, the processor deals with exceptions in
the fixed order given in Exception priorities on page 2-43.

This section provides details of the processor exception handling:
• Exception entry and exit summary
• Leaving an exception on page 2-36.
• Reset on page 2-36
• Fast interrupt request on page 2-37
• Interrupt request on page 2-37
• Aborts on page 2-38
• Imprecise data abort mask in the CPSR/SPSR on page 2-40
• Software interrupt instruction on page 2-41
• Software Monitor Instruction on page 2-41
• Undefined instruction on page 2-41
• Breakpoint instruction on page 2-42
• Exception vectors on page 2-42
• Exception priorities on page 2-43

(See Table 2-12 Exception Entry and Exit)

#### 2.15.5 Interrupt Request
The IRQ exception is a normal interrupt caused by a LOW level on the nIRQ input. IRQ
has a lower priority than FIQ, and is masked on entry to an FIQ sequence.
Irrespective of whether exception entry is from ARM state, Thumb state, or Java state,
an IRQ handler returns from the interrupt by executing:
SUBS PC,R14_irq,#4
You can disable IRQ exceptions within a privileged mode by setting the CPSR I flag.
When the I flag is cleared to 0, the processor checks for a LOW level on the output of
the nIRQ register at the end of each instruction.
IRQs are disabled when an IRQ occurs. You can use nested interrupts but it is up to you
to save any corruptible registers and to re-enable IRQs.
The IRQ bit in the SCR register configures the IRQ to branch to either the current IRQ
mode or to the Monitor mode.

#### 2.15.12 Exception Vectors
The Secure Configuration Register bits [3:1] determine which mode is entered when an
IRQ, a FIQ, or an external abort exception occurs. The CP15 c12, Secure or Nonsecure
Vector Base Address Register and the Monitor Vector Base Address Register define the
base address of the Nonsecure, Secure, and Secure Monitor vector tables. If high vectors
are enabled using CP15 c1 bit[13], the base address of the Nonsecure and Secure vector
tables is 0xFFFF0000, regardless of the value of these registers. Enabling high vectors has
no effect on the Secure Monitor vector addresses.