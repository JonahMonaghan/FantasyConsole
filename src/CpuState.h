//
// Created by Jonah on 10/13/2025.
//

#pragma once

#include <cstdint>

struct CpuState{
    //GPRs
    uint8_t GPR[8]; //R0 through R7

    //SPRs
    uint16_t IP; //Instruction Pointer
    uint16_t SP; //Stack Pointer

    //Flags
    //Z - Zero Flag if the result is exactly zero
    //C - Carry Flag if the operation resulted in an overflow
    uint8_t FLAGS;
};

enum class Flag : uint8_t {
    CARRY = 0,
    ZERO = 1
};