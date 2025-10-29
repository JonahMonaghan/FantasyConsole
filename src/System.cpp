//
// Created by Jonah on 10/17/2025.
//

#include "System.h"
#include <iostream>

System::System()
    : m_opcode_handler(*this){
    reset();
}

void System::run(){
    while(m_is_running){
        step();
    }
}

void System::reset(){
    for(uint8_t& reg: m_cpu.GPR){
        reg = 0x00;
    }
    m_cpu.IP = 0x0100;
    m_cpu.SP = 0x9FFF;

    m_ram.fill(0x00);

    m_is_running = true;
}

void System::step() {
    m_opcode_handler.execute();
}

uint8_t System::readMemory(uint16_t address) {
    return m_ram[address];
}

void System::writeMemory(uint16_t address, uint8_t value) {
    m_ram[address] = value;
}

uint8_t System::fetchByte() {
    return m_ram[m_cpu.IP++];
}

uint16_t System::fetchAddress() {
    uint8_t low_byte = fetchByte();
    uint8_t high_byte = fetchByte();
    return mergeBytes(low_byte, high_byte);
}

uint16_t System::mergeBytes(uint8_t low_byte, uint8_t high_byte){
    return (high_byte << 8) | low_byte;
}