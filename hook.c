// tl;dr: clang hook.c core.ll -o a.out
//
// Create IR hook.ll from this file, assemble to bytecode hook.bc
//
//     clang -emit-llvm -S -c hook.c -o hook.ll
//     llvm-as hook.ll -o hook.bc
//
// Codegen'ed core.ll, assemble to bytecode core.bc
//
//     ./build/Release/core 2> core.ll
//     llvm-as core.ll -o core.bc
//
// Link bytecode files core.bc and hook.bc, check IR
//
//     llvm-link hook.ll core.ll -o a.out.bc
//     llvm-dis a.out.bc -o a.out.ll
//
// Compile bytecode to ELF executable, check return code
//
//     clang a.out.bc -o a.out
//     ./a.out; echo $?

#include <stdint.h>

// defined by codegen
extern int32_t fn();

int main() { return fn(); }
