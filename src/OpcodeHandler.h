//
// Created by Jonah on 10/28/2025.
//

#pragma once

#include "CpuState.h"
#include "System.h"
#include <array>
#include <cstdint>

class System;

class OpcodeHandler {

    using HandlerFunc = void (OpcodeHandler::*)();

private:
    System& m_system;
    std::array<HandlerFunc, 256> m_dispatch_table;


    void op_NOP();
    void op_LD_IMM();
    void op_PRT_REG();
    void op_MOV_REG_REG();
    void op_ADD_REG_REG();
    void op_JZ();
    void op_JMP();
    void op_JNZ();
    void op_LD_MEM();
    void op_ST_MEM();
    void op_PUSH_REG();
    void op_POP_REG();
    void op_HALT();
    void op_UNKNOWN();

    void initializeDispatchTable();

public:
    OpcodeHandler(System& system);

    bool execute();
};