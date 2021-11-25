# How to work with this processor? 

## 1. Write code.txt using commands from cmd.h

### Assembler example:

*This code prints fibonacci numbers with values under 1000*

```push 1
push 1

pop ax
pop bx

jump :1

:1
push ax
push bx

add

pop cx // сумму в сх

push bx
pop ax // перенос значения бх в ах

push cx
out
pop bx // перенос значения сх в бх

push 1000
push cx
jbe :1
```

#### Features:
* Any Line MUST start with command /or/ be empty)
* You can store values in any of 4 (!!!) registers [ax, bx, cx, dx]:
```
push 2017
pop ax
push ax
out 

>>> 2017
```
* Or in RAM:
```
push 2021
pop [0] // puts in 1st RAM's cell
push [0]
out

>>> 2021
```
* Assembler checks line by line: checks command, checks its arguments. 

Thus after command (and argument if command has it) you can write comments:
```
push 23 // Birthday day
push 9 // Birthday month
pop ax // Put month in ax
pop bx // Put month in bx
```

## 2. Run .bat files

Run asma_maker.bat, then proc_maker.bat

# Well done!
