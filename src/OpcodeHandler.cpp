//
// Created by Jonah on 10/28/2025.
//

#include "OpcodeHandler.h"
#include <iostream>

OpcodeHandler::OpcodeHandler(System& system)
    : m_system(system){
    initializeDispatchTable();
}

bool OpcodeHandler::execute() {
    uint8_t opcode = m_system.fetchByte();
    HandlerFunc handler = m_dispatch_table[opcode];
    (this->*handler)();
    return m_system.m_is_running;
}

void OpcodeHandler::op_POP_REG() {
    auto& gpr = m_system.m_cpu.GPR;
    auto& sp = m_system.m_cpu.SP;

    uint8_t dest_reg = m_system.fetchByte();
    gpr[dest_reg] = m_system.m_ram[sp];
    sp++;
}

void OpcodeHandler::op_PUSH_REG() {
    auto& gpr = m_system.m_cpu.GPR;
    auto& sp = m_system.m_cpu.SP;

    uint8_t src_reg = m_system.fetchByte();
    sp--;
    m_system.m_ram[sp] = gpr[src_reg];
}

void OpcodeHandler::op_ST_MEM() {
    auto& gpr = m_system.m_cpu.GPR;

    uint8_t src_reg = m_system.fetchByte();
    uint16_t mem_addr = m_system.fetchAddress();

    m_system.m_ram[mem_addr] = gpr[src_reg];
}

void OpcodeHandler::op_LD_MEM() {
    auto& gpr = m_system.m_cpu.GPR;

    uint8_t dest_reg = m_system.fetchByte();
    uint16_t mem_addr = m_system.fetchAddress();

    gpr[dest_reg] = m_system.m_ram[mem_addr];
}

void OpcodeHandler::op_JNZ() {
    auto& ip = m_system.m_cpu.IP;
    auto& flags = m_system.m_cpu.FLAGS;

    uint16_t jmp_addr = m_system.fetchAddress();

    if(!(flags & (1 << static_cast<uint8_t>(Flag::ZERO)))){
        ip = jmp_addr;
    }
}

void OpcodeHandler::op_JMP() {
    auto& ip = m_system.m_cpu.IP;

    uint16_t jmp_addr = m_system.fetchAddress();

    ip = jmp_addr;
}

void OpcodeHandler::op_JZ(){
    auto& ip = m_system.m_cpu.IP;
    auto& flags = m_system.m_cpu.FLAGS;

    uint16_t jmp_addr = m_system.fetchAddress();

    if(flags & (1 << static_cast<uint8_t>(Flag::ZERO))){
        ip = jmp_addr;
    }
}

void OpcodeHandler::op_ADD_REG_REG() {
    auto& gpr = m_system.m_cpu.GPR;
    auto& flags = m_system.m_cpu.FLAGS;

    uint8_t src_reg = m_system.fetchByte();
    uint8_t dest_reg = m_system.fetchByte();
    uint16_t result = (uint16_t)gpr[src_reg] + (uint16_t)gpr[dest_reg];

    //Clear the flags
    flags &= ~(1 << static_cast<uint8_t>(Flag::CARRY));
    flags &= ~(1 << static_cast<uint8_t>(Flag::ZERO));
    if(result > 0xFF){
        flags |= (1 << static_cast<uint8_t>(Flag::CARRY));
    }

    if((result & 0xFF) == 0){
        flags |= (1 << static_cast<uint8_t>(Flag::ZERO));
    }

    gpr[dest_reg] = (uint8_t)result;
}

void OpcodeHandler::op_MOV_REG_REG() {
    auto& gpr = m_system.m_cpu.GPR;

    uint8_t src_reg = m_system.fetchByte();
    uint8_t dest_reg = m_system.fetchByte();
    gpr[dest_reg] = gpr[src_reg];
}

void OpcodeHandler::op_PRT_REG() {
    auto& gpr = m_system.m_cpu.GPR;

    uint8_t src_reg = m_system.fetchByte();
    uint8_t value = gpr[src_reg];
    std::cout << "PRT_REG - R" << (int)src_reg << " Value: " << std::hex << (int)value << std::endl;
}

void OpcodeHandler::op_LD_IMM() {
    auto& gpr = m_system.m_cpu.GPR;

    uint8_t reg_index = m_system.fetchByte();
    uint8_t value = m_system.fetchByte();
    gpr[reg_index] = value;
}

void OpcodeHandler::op_NOP() {
    return;
}

void OpcodeHandler::op_HALT() {
    m_system.m_is_running = false;
}

void OpcodeHandler::op_UNKNOWN() {
    std::cerr << "Error: Unknown opcode";
    m_system.m_is_running = false;
}

void OpcodeHandler::initializeDispatchTable(){
    m_dispatch_table.fill(&OpcodeHandler::op_UNKNOWN);

    m_dispatch_table[0x00] = &OpcodeHandler::op_NOP;
    m_dispatch_table[0x10] = &OpcodeHandler::op_LD_IMM;
    m_dispatch_table[0x20] = &OpcodeHandler::op_PRT_REG;
    m_dispatch_table[0x30] = &OpcodeHandler::op_MOV_REG_REG;
    m_dispatch_table[0x40] = &OpcodeHandler::op_ADD_REG_REG;
    m_dispatch_table[0x50] = &OpcodeHandler::op_JZ;
    m_dispatch_table[0x51] = &OpcodeHandler::op_JMP;
    m_dispatch_table[0x52] = &OpcodeHandler::op_JNZ;
    m_dispatch_table[0x60] = &OpcodeHandler::op_LD_MEM;
    m_dispatch_table[0x70] = &OpcodeHandler::op_ST_MEM;
    m_dispatch_table[0x80] = &OpcodeHandler::op_PUSH_REG;
    m_dispatch_table[0x81] = &OpcodeHandler::op_POP_REG;
    m_dispatch_table[0xFF] = &OpcodeHandler::op_HALT;
}