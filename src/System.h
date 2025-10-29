//
// Created by Jonah on 10/17/2025.
//

#pragma once
#include "CpuState.h"
#include "OpcodeHandler.h"
#include <array>
#include <cstdint>

class OpcodeHandler;

class System{
private:

    friend class OpcodeHandler;

    CpuState m_cpu;
    std::array<uint8_t, 65536> m_ram;

    OpcodeHandler m_opcode_handler;

    bool m_is_running;

    uint8_t fetchByte();
    uint16_t fetchAddress();
    static uint16_t mergeBytes(uint8_t low_byte, uint8_t high_byte);

public:
    System();

    void run();

    void reset();
    void step();

    uint8_t readMemory(uint16_t address);
    void writeMemory(uint16_t address, uint8_t value);

    bool isRunning() const {return m_is_running;}
};