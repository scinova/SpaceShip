# 1 "ws2812b.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "ws2812b.S"

# 1 "/usr/lib/avr/include/avr/io.h" 1 3
# 99 "/usr/lib/avr/include/avr/io.h" 3
# 1 "/usr/lib/avr/include/avr/sfr_defs.h" 1 3
# 100 "/usr/lib/avr/include/avr/io.h" 2 3
# 174 "/usr/lib/avr/include/avr/io.h" 3
# 1 "/usr/lib/avr/include/avr/iom2560.h" 1 3
# 1578 "/usr/lib/avr/include/avr/iom2560.h" 3
       
# 1579 "/usr/lib/avr/include/avr/iom2560.h" 3

       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
# 175 "/usr/lib/avr/include/avr/io.h" 2 3
# 627 "/usr/lib/avr/include/avr/io.h" 3
# 1 "/usr/lib/avr/include/avr/portpins.h" 1 3
# 628 "/usr/lib/avr/include/avr/io.h" 2 3

# 1 "/usr/lib/avr/include/avr/common.h" 1 3
# 630 "/usr/lib/avr/include/avr/io.h" 2 3

# 1 "/usr/lib/avr/include/avr/version.h" 1 3
# 632 "/usr/lib/avr/include/avr/io.h" 2 3






# 1 "/usr/lib/avr/include/avr/fuse.h" 1 3
# 639 "/usr/lib/avr/include/avr/io.h" 2 3


# 1 "/usr/lib/avr/include/avr/lock.h" 1 3
# 642 "/usr/lib/avr/include/avr/io.h" 2 3
# 3 "ws2812b.S" 2

.equ OUTBIT, 6
.equ PORT, ((0x05) + 0)
.equ BIT, 6

.global ws2812b_writegrb

ws2812b_writegrb:
 movw r26, r24 ;r26:27 = X = p_buf
 movw r24, r22 ;r24:25 = count
 in r22, ((0x3F) + 0) ;save ((0x3F) + 0) (global int state)
 cli ;no interrupts from here on, we're cycle-counting
 in r20, PORT
 ori r20, (1 << (BIT)) ;our '1' output
 in r21, PORT
 andi r21, ~(1 << (BIT)) ;our '0' output
 ldi r19, 7 ;7 bit counter (8th bit is different)
 ld r18, X+ ;get first data byte

;0.125us
;0.0625us 6.4 / 13.6 3.2/6.8 3/7 6 14
; 12.8 / 7.2 6.4/3.6 7/3 13 7

loop1:
 out PORT, r20 ; 1 +0 start of a bit pulse
 lsl r18 ; 1 +1 next bit into C, MSB first
 brcs L1 ; 1/2 +2 branch if 1
 rjmp .+0 ; 2 +4
 nop ; 1 +5
 out PORT, r21 ; 1 +3 +6 end hi for '0' bit (3 clocks hi)
 rjmp .+0 ; 2 +5 +8
 rjmp .+0 ; 2 +10
 rjmp .+0 ; 2 +12
 rjmp .+0 ; 2 +14
 bst r18, 7 ; 1 +5 +15 save last bit of data for fast branching
 subi r19, 1 ; 1 +6 +16 how many more bits for this byte?
 breq bit8 ; 1/2 +7 +17 last bit, do differently
 rjmp loop1 ; 2 +8 +18 10 total for 0 bit
L1:
 nop ; 1 +4 +4
 rjmp .+0 ; X2 +5
 rjmp .+0 ; X2 +7
 rjmp .+0 ; X2 +9
 bst r18, 7 ; 1 +5 +11 save last bit of data for fast branching
 subi r19, 1 ; 1 +6 +12 how many more bits for this byte
 out PORT, r21 ; 1 +7 +13 end hi for '1' bit (7 clocks hi)
 rjmp .+0 ; 2 +16
 nop ; X +17
 brne loop1 ; 2/1 +8 +18 10 total for 1 bit (fall thru if last bit)
bit8:
 ldi r19, 7 ; 1 +9 bit count for next byte
 out PORT, r20 ; 1 +0 start of a bit pulse
 brts L2 ; 1/2 +1 branch if last bit is a 1
 rjmp .+0 ; 2 +3
 rjmp .+0 ; 2 +5
 out PORT, r21 ; 1 +3 +6 end hi for '0' bit (3 clocks hi)
 rjmp .+0 ; 2 +7
 rjmp .+0 ; 2 +9
 rjmp .+0 ; 2 +11
 nop ; 1 +12

 ld r18, X+ ; 2 +4 +14 fetch next byte
 sbiw r24, 1 ; 2 +6 +16 dec byte counter
 brne loop1 ; 2 +8 +18 loop back or return
 out ((0x3F) + 0), r22 ; restore global int flag
 ret
L2:
 ld r18, X+ ; 2 +3 fetch next byte
 sbiw r24, 1 ; 2 +5 dec byte counter
 nop ; 1 +6
 rjmp .+0 ; 2 +8
 rjmp .+0 ; 2 +10
 rjmp .+0 ; 2 +12
 out PORT, r21 ; 1 +14 end hi for '1' bit (7 clocks hi)
 nop ; 1 +15
 rjmp .+0 ; 1 +17
 brne loop1 ; 2 +8 +18 loop back or return
 out ((0x3F) + 0), r22 ; restore global int flag
 ret
