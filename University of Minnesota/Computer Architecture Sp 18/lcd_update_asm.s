# Deandra Bardell
# Created Feb 26 18

# rax rcx rdx rdi rsi r8 r9 r10 r11  # Caller save registers

# Note: Throughout in my comments I mention line numbers. These correspond to my lcd_update.c file. I follow my C code closely, so I hope my assembly is clear. I use r8 & r9 a lot for temporary memory, like in division and masking. r10 & r11 tend to hold data I need to keep, like the state variable. These conventions are not strict. Error blocks are .ERR, .ERROR, .ERROR2.

.text
.global  set_tod_from_secs

## ENTRY POINT FOR REQUIRED FUNCTION
set_tod_from_secs:
        # edi has time_of_day_sec, rsi has *tod

  cmpl $86400,%edi        # time_of_day_sec >8600
  jg .ERR                 # jump to error
  cmpl $0,%edi            # time_of_day_sec <0
  jl .ERR                 # jump to error

  movl $0,%edx            # edx=0 for division
  movl %edi,%eax          # time_of_day_sec is numerator. so eax
  cqto                    # carry sign for division
  movl $3600,%r8d         # divisor is 3600 sec, to r8d
  idivl %r8d              # time_of_day_sec/3600
  movw %ax,0(%rsi)        # quotient to tod->hours
  movl %edx, %r11d        # remainder to r11d, extrahour in C code

  cmpl $0,%eax            # if tod->hours != 0, means not 12am
  jne .NOT12AM            # jump to next conditional
  movl $12,0(%rsi)        # it's 12am, so tod->hours=12
  movb $0, 6(%rsi)        # ispm=0
  jmp .RESTOF             # jump out of conditional, to rest of code

.NOT12AM:                 # if tod->hours >12 in C code, line 26
  cmpl $12, %eax          # if tod->hours<= 12
  jle  .LESSEQUAL12       # jump to next conditional
  subl $12,0(%rsi)        # hours=hours-12, since we are 12 hour time
  movb $1,6(%rsi)         # ispm=1
  jmp .RESTOF             # jump out of conditional, to rest of code

.LESSEQUAL12:             # if tod-hours==12 in C code, line 29
  cmpl $12,%eax           # if tod->hours != 12
  jne .NOT12              # jump to next conditional
  movb $1,6(%rsi)         # ispm=1
  jmp .RESTOF             # jump out of conditional, to rest of code

.NOT12:                   # last else in C code, line 31
  movb $0,6(%rsi)         # ispm=0

.RESTOF:
  movl $0,%edx            # for division
  movl %r11d,%eax         # remainder from earlier is numerator, eax
  cqto                    # extend sign
  movl $60,%r8d           # divide by 60
  idivl %r8d              # remainder/60
  movw %ax, 2(%rsi)       # ax has minutes, put in tod->minutes
  movw %dx,4(%rsi)        # dx has seconds, put in tod->seconds
  movl $0,%eax            # return 0
  ret

.ERR:
  movl $1,%eax            # there's an error, return 1
  ret

### Data area associated with the next function
.data

state:                    # state var we use to build up lcd bit pattern
        .int 0b0000000000000000000000000000000000000

arr_time_bits:               # array of lcd bit patterns
        .int 0b0111111  # 0
        .int 0b0000110  # 1
        .int 0b1011011  # 2
        .int 0b1001111  # 3
        .int 0b1100110  # 4
        .int 0b1101101  # 5
        .int 0b1111101  # 6
        .int 0b0000111  # 7
        .int 0b1111111  # 8
        .int 0b1101111  # 9
        .int 0b0000000  # blank


.text
.global  set_display_bits_from_tod

## ENTRY POINT FOR REQUIRED FUNCTION
set_display_bits_from_tod:
        ## assembly instructions here

	## two useful techniques for this problem
  movl    state(%rip),%r10d    # load state into register r10d
  leaq    arr_time_bits(%rip),%r11  # load pointer to beginning of arr_time_bits into r11

  # tod in rdi   *display in rsi

  movq %rdi,%r9              # tod to r9 for masking
  andl $0xFFFF, %r9d         # mask hours, in r9d
  cmpl $12,%r9d              # if hours> 12
  jg .ERROR                  # jump to error

  movq %rdi,%r9              # tod to r9 for mask
  sar $16,%r9                # shift right to mins
  andl $0xFFFF, %r9d         # mask mins info in r9d
  cmpl $59,%r9d              # if mins>59
  jg .ERROR                  # jump to error

  movq %rdi,%r9              # for mask
  sar $32,%r9                # shift right to seconds
  andl $0xFFFF, %r9d         # mask seconds info
  cmpl $59,%r9d              # if seconds >59
  jg .ERROR                  # jump to error

  movq %rdi,%r9              # for mask
  sar $48,%r9                # shift right to ispm
  andl $0xFF, %r9d           # mask ispm info
  cmpl $1,%r9d               # if ispm>1
  jg .ERROR                  # jump to error

  movq %rdi,%r9              # tod to r9
  sar $48,%r9                # shift right to ispm
  andl $0xFF,%r9d            # mask ispm info
  testl %r9d,%r9d            # if (ispm) in C, line 75
  jne .PM                    # jump to set pm conditional

  or $1,%r10d                # set bits to am by or
  jmp .RESTOF2               # jump to rest of code

.PM:
  or $2,%r10d                # set bits to pm

.RESTOF2:
  sal $7,%r10d               # << state by 7, state <<7
  movq %rdi,%rcx             # for mask
  andl $0xFFFF, %ecx         # mask hours info
  cmpl $9,%ecx               # if hours <=9, line 84 in C
  jle .HOURSLESSEQ9          # jump to elso conditional, hours 1-9

  movq $1,%r9                # arr_time_bits index needed is 1
  movl (%r11,%r9,4),%r8d     # arr_time_bits[1] to r8d
  or %r8d,%r10d              # or the bit pattern of 1 with state
  sal $7,%r10d               # state<<7

  movl $0,%edx               # for division
  movl %ecx,%eax             # ecx still has hours of tod, to numerator
  cqto                       # extend sign
  movl $10,%r8d              # divide by 10
  idivl %r8d                 # hours/10
  movq %rdx,%r9              # move remainder to r9, it's the index
  movl (%r11,%r9,4),%r8d     # arr_time_bits[remainder] to r8d
  or %r8d,%r10d              # or bit pattern with state
  jmp .MINSEC                # jump to minutes & seconds code

.HOURSLESSEQ9:               # hours are 1-9,  line 91  else
  movq $10,%r9               # index of blank lcd bits, tens place
  movl (%r11,%r9,4),%r8d     # arr_time_bits[10] to r8d
  or %r8d,%r10d              # or bit pattern with state
  sal $7,%r10d               # state << 7

  movq %rdi,%r9              # tod to r9 for masking
  and $0xFFFF, %r9           # mask hours bits in r9
  movl (%r11,%r9,4),%r8d     # arr_time_bits[hours] to r8d
  or %r8d,%r10d              # or bit pattern with state

.MINSEC:                     # C code line 98, mins & secs bits
  sal $7,%r10d               # state <<7

  movl $0,%edx               # for division
  movq %rdi,%r9              # for masking
  sar $16,%r9                # shift right to mins
  and $0xFFFF,%r9            # mask mins info in r9
  movl %r9d,%eax             # minutes is numerator, eax
  cqto                       # extend sign
  movl $10,%r8d              # divide by 10
  idivl %r8d                 # minutes/10
  movq %rax,%r9              # quotient in rax is array index
  movl (%r11,%r9,4),%r8d     # arr_time_bits[quotient] to r8d
  or %r8d,%r10d              # or bit pattern with state

  sal $7,%r10d               # state <<7
  movq %rdx,%r9              # remainder of div to r9, mins ones
                                # place is array index
  movl (%r11,%r9,4),%r8d     # arr_time_bits[remainder] to r8d
  or %r8d,%r10d              # or bit pattern with state

  movl %r10d,(%rsi)          # move state to second arg pointer,
                                # *display=state   C line 103
  movl $0,%eax               # return 0
  ret

.ERROR:
  movl $1,%eax               # there's an error, return 1
  ret



 .text
 .global lcd_update

 ## ENTRY POINT FOR REQUIRED FUNCTION
lcd_update:
 	## assembly instructions here

         ## a useful technique for this problem
  movl TIME_OF_DAY_SEC(%rip), %edi  # load global variable into edi

   pushq $0                   # empty tod on stack
   movq %rsp,%rsi             # put pointer to empty tod in rsi
   call set_tod_from_secs     # edi first arg, rsi second arg

   cmpl $1,%eax               # check if set_tod_from_secs returned 1
   je .ERROR2                 # jump to error if yes

   movq (%rsi),%rdi           # moved packed tod as first arg
   leaq LCD_DISPLAY_PORT(%rip), %rsi  # pointer to LCD_DISPLAY_PORT as second arg

   call set_display_bits_from_tod  # rdi first arg, rsi second arg

   cmpl $1,%eax               # check if set_display_bits_from_tod returned 1
   je .ERROR2                 # jump to error if yes

   popq %r9                   # pop off stack, any register

   movl $0,%eax               # return 0
   ret

 .ERROR2:                     # there's an error
   popq %r9                   # pop off stack, any register
   movl $1,%eax               # return 1
   ret
