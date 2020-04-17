(module
(import "host" "exit" (func $halt))
(import "host" "getchar" (func $getchar (result i32)))
(import "host" "putchar" (func $printc (param i32)))
(start $main)
(global $G0 (mut i32) (i32.const 0))
(global $G1 (mut i32) (i32.const 0))
(global $G2 (mut i32) (i32.const 0))
(global $G3 (mut i32) (i32.const 0))
(global $G4 (mut i32) (i32.const 0))
(global $G5 (mut i32) (i32.const 0))
(global $G6 (mut i32) (i32.const 0))
(global $G7 (mut i32) (i32.const 0))
(global $G8 (mut i32) (i32.const 0))
(global $G9 (mut i32) (i32.const 0))
(global $G10 (mut i32) (i32.const 0))
(global $G11 (mut i32) (i32.const 0))
(global $G12 (mut i32) (i32.const 0))
(global $G13 (mut i32) (i32.const 0))
(global $G14 (mut i32) (i32.const 0))
(global $G15 (mut i32) (i32.const 0))
(global $G16 (mut i32) (i32.const 0))
(global $G17 (mut i32) (i32.const 0))
(global $G18 (mut i32) (i32.const 0))
(global $G19 (mut i32) (i32.const 0))
(global $G20 (mut i32) (i32.const 0))
(global $G21 (mut i32) (i32.const 0))
(global $G22 (mut i32) (i32.const 0))
(global $G23 (mut i32) (i32.const 0))
(global $G24 (mut i32) (i32.const 0))
(global $G25 (mut i32) (i32.const 0))
(global $G26 (mut i32) (i32.const 0))
(func $F0
(result i32)
get_global $G0
if
i32.const 0
set_global $G0
get_global $G1
return
end
call $getchar
return
)
(func $F1
(param $P0 i32)
get_global $G0
if
i32.const 7
i32.const 40
call $prints
call $halt
end
i32.const 1
set_global $G0
get_local $P0
set_global $G1
)
(func $F2
(result i32)
get_global $G2
if
end
i32.const 1
set_global $G2
call $F4
set_global $G3
get_global $G3
return
)
(func $F3
(param $P0 i32)
call $F2
get_local $P0
i32.ne
if
i32.const 40
i32.const 55
call $prints
get_local $P0
call $printc
i32.const 55
i32.const 56
call $prints
call $halt
end
i32.const 0
set_global $G2
)
(func $F4
(result i32)
(local $L0 i32)
(block $B1
(loop $W1
call $F0
set_local $L0
get_local $L0
call $F6
br_if $W1
)
)
get_local $L0
get_global $G5
i32.eq
if
end
get_local $L0
get_global $G26
i32.eq
if
end
get_local $L0
get_global $G17
i32.eq
if
end
get_local $L0
get_global $G18
i32.eq
if
end
get_local $L0
get_global $G19
i32.eq
if
end
get_local $L0
get_global $G20
i32.eq
if
end
get_local $L0
get_global $G21
i32.eq
if
end
get_local $L0
get_global $G22
i32.eq
if
end
get_local $L0
call $F5
if
i32.const 0
set_global $G4
(block $B1
(loop $W1
get_global $G4
i32.const 10
i32.mul
get_local $L0
get_global $G15
i32.sub
i32.add
set_global $G4
call $F0
set_local $L0
get_local $L0
call $F5
br_if $W1
)
)
get_local $L0
call $F1
get_global $G8
return
end
i32.const 56
i32.const 82
call $prints
call $halt
)
(func $F5
(param $P0 i32)
(result i32)
get_local $P0
get_global $G15
i32.ge_s
get_local $P0
get_global $G16
i32.le_s
i32.and
return
)
(func $F6
(param $P0 i32)
(result i32)
get_local $P0
get_global $G23
i32.eq
get_local $P0
get_global $G25
i32.eq
i32.or
get_local $P0
get_global $G24
i32.eq
i32.or
return
)
(func $F7
(local $L0 i32)
(block $B1
(loop $W1
call $F8
set_local $L0
get_global $G7
call $F3
i32.const 82
i32.const 84
call $prints
get_local $L0
call $printi
i32.const 84
i32.const 85
call $prints
call $F2
get_global $G6
i32.ne
br_if $W1
)
)
)
(func $F8
(result i32)
(local $L0 i32)
(local $L1 i32)
(local $L2 i32)
call $F9
set_local $L0
(block $B1
(loop $W1
call $F2
set_local $L2
get_local $L2
call $F3
call $F9
set_local $L1
get_local $L2
get_global $G9
i32.eq
if
else
end
call $F2
get_global $G9
i32.eq
call $F2
get_global $G10
i32.eq
i32.or
br_if $W1
)
)
get_local $L0
return
)
(func $F9
(result i32)
(local $L0 i32)
(local $L1 i32)
(local $L2 i32)
call $F10
set_local $L0
(block $B1
(loop $W1
call $F2
set_local $L2
get_local $L2
call $F3
call $F10
set_local $L1
get_local $L2
get_global $G11
i32.eq
if
else
end
call $F2
get_global $G11
i32.eq
call $F2
get_global $G12
i32.eq
i32.or
br_if $W1
)
)
get_local $L0
return
)
(func $F10
(result i32)
(local $L0 i32)
(local $L1 i32)
call $F2
set_local $L1
get_local $L1
get_global $G13
i32.eq
if
get_global $G13
call $F3
call $F8
set_local $L0
get_global $G14
call $F3
else
get_local $L1
get_global $G10
i32.eq
if
get_global $G10
call $F3
call $F10
i32.const -1
i32.mul
set_local $L0
else
get_local $L1
get_global $G8
i32.eq
if
get_global $G8
call $F3
get_global $G4
set_local $L0
else
i32.const 85
i32.const 109
call $prints
call $halt
end
end
end
get_local $L0
return
)
(func $F11
i32.const 1
i32.const -1
i32.mul
set_global $G5
i32.const 48
set_global $G15
i32.const 57
set_global $G16
i32.const 43
set_global $G17
i32.const 45
set_global $G18
i32.const 42
set_global $G19
i32.const 47
set_global $G20
i32.const 40
set_global $G21
i32.const 41
set_global $G22
i32.const 32
set_global $G23
i32.const 9
set_global $G24
i32.const 13
set_global $G25
i32.const 10
set_global $G26
i32.const 256
set_global $G6
i32.const 257
set_global $G8
get_global $G17
set_global $G9
get_global $G18
set_global $G10
get_global $G19
set_global $G11
get_global $G20
set_global $G12
get_global $G21
set_global $G13
get_global $G22
set_global $G14
get_global $G26
set_global $G7
)
(func $main
call $F11
call $F7
)

(func $printi (param $i i32) (local $n i32)
(i32.eqz
(get_local $i))
if ;; if (i == 0)
i32.const 48
call $printc ;; print '0'
else ;; (i != 0)
;; n = 1
i32.const 10
set_local $n
;; while (i / n > 0)
loop
(i32.eqz
(i32.div_s
(get_local $i)
(get_local $n)))
if ;; if (i / n == 0) Found cap n (n digits)
loop
;; while (n != 1)
;; n /= 10
(i32.div_s
(get_local $n)
(i32.const 10))
set_local $n
;; putchar (print left most digit)
(i32.add
(i32.const 48)
;; offset for ascii digits
(i32.div_s 
;; $i / $n (integer division)
(get_local $i)
(get_local $n)))
call $printc
;; i = i - (i / n) * n (this gets rid of the highest order digit in i)
(i32.sub
(get_local $i)
(i32.mul
(i32.div_s
(get_local $i)
(get_local $n))
(get_local $n)))
set_local $i
;; loop statement
(i32.ne
(get_local $n)
(i32.const 1))
br_if 0
return
end
end
;; Cap isn't found yet
;; n *= 10
(i32.mul
(get_local $n)
(i32.const 10))
set_local $n
br 0
end
end);; memory start location
;; length of string (how many bytes)
(func $prints (param $start i32) (param $bytes i32)
;; current byte
(local $i i32) 
;; every 4 bytes increase by 1 (load only loads 4 bytes at a time, gotta keep track of this)
(local $offset i32)
;; Initialize locals
i32.const 0
set_local $i
i32.const 0
set_local $offset
(loop $charLoop
;; loaded = load(start + offset) - loaded
(i32.load
(i32.add
(get_local $start)
(i32.mul
(get_local $offset)
(i32.const 4))))
;; loaded = loaded % (2^( 8 * (i+1) )  )
;; removes higher order bits
(i32.sub
(i32.const 24)
(i32.mul
(i32.const 8)
(get_local $i)))
i32.shl
i32.const 24
i32.shr_u
call $printc
;; i += 1
(i32.add
(get_local $i)
(i32.const 1))
set_local $i
get_local $i
i32.const 4
i32.ge_s
;; if (i >= 4) {
;;	i = 0
;;	offset += 1
;; }
if
i32.const 0
set_local $i
(i32.add
(get_local $offset)
(i32.const 1))
set_local $offset
end
;; bytes <= (offset * 4) + i
(i32.add
(i32.mul
(get_local $offset)
(i32.const 4))
(get_local $i))
get_local $bytes
i32.lt_s
br_if $charLoop))(func $printb (param $bool i32)
get_local $bool
i32.eqz
if
i32.const 4
i32.const 5
call $prints
else
i32.const 0
i32.const 4
call $prints
end)

(data 0 (i32.const 0) "true")
(data 0 (i32.const 3) "false")
(data 0 (i32.const 7) "Internal error: too many ungets!\n")
(data 0 (i32.const 40) "Error: expected ")
(data 0 (i32.const 55) "\n")
(data 0 (i32.const 56) "Error: invalid character.\n")
(data 0 (i32.const 82) " = ")
(data 0 (i32.const 84) "\n")
(data 0 (i32.const 85) "Error: expected factor.\n")
(memory 1)
)