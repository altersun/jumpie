# Jump Tables in C and C++
Might as well jump 🎵

## What's a Jump Table?
Also called a "Branch Table," jump tables are ways of arranging functions into a table for easy access based on an integral key. 

## ...ok, *why* a Jump Table?
Work long enough in C or C++ and you will inevitably come across legacy code built around *enormous* switch statements. I'm talking tens if not *hundreds* of cases in the switch, and it scrolls on for *thousands* of lines or more. Gross!

The advantage of a jump table is that, if each case is refactored into a function of a common prototype, pointers to each function can be placed into an array where what was the value for selecting the case is now the index into the array. Much easier to read and much easier to maintain.

## So what's here?
Macros for jump tables in C, and templates for jump tables in C++.

### C
Only integral keys are supported. There's macros for three kinds of jump tables:
1. A simple table which is just an array of functions. Just provide the name of the array, the function pointer type, and the functions, and you're done.
2. A complex table which can support a *sub*array of functions. I once saw a colleage implement a jump table with columns for which function would be used per command, depending on the system version, which was the inspiriation for this.
3. A jump *menu*, where each function is paired with a descriptive string. I've found this pattern useful in composing debug menus for embedded systems with serial interaces. Also comes with a handy menu print macro!

### C++
Heavily uses C++ concepts for force certain use cases. The keys can be any type, but the values of the table *must* be functions, though the prototype can be anything. In the case of integral keys (so enum, int, char, etc), a vector is used under the hood. For orther key types, a map is used under the hood.

So why not use a map or vector directly? With the jump table temple, there are some restrictions to make sure it's used only for its specific purpose:
1. If the value is a non-functional type, compilation will fail (thanks, C++ concepts!)
2. Jump tables are, in every case I've seen, static. This implementation forces a static, constant container that can't be updated after compilation.

## Can I try it?
Sure! There are 3 test programs for the c variants and 1 for the cpp template. Build 'em all with a simple `make` and run them from the resulting `build` directory.
