#ifndef __AVR_HPP__
#define __AVR_HPP__

#include "../frontend.hpp"
// #include "../uops.hpp"
#include "../slots.hpp"
#include <vector>
#include <string>
#include <string.h>

class AvrMachine : public TargetMachine
{
public:
    AvrMachine()
    {
        for(int i = 0; i < 32; ++i)
            registers[i] = 0;
    }

    uint8_t getRegister(Slot *reg)
    {
        return registers[reg->getValue()];
    }

    void setRegister(Slot *reg, uint8_t value)
    {
        registers[reg->getValue()] = value;
    }


    uint8_t getRegisterValue(unsigned reg)
    {
        return registers[reg];
    }

    void setRegisterValue(unsigned reg, uint8_t value)
    {
        registers[reg] = value;
    }

    std::string toString()
    {
        std::string s;

        for(int i = 0; i < 32; ++i)
        {
            s += "\t";
            s += "r" + std::to_string(i) + " = " + std::to_string(registers[i]) + "  ";
            if(i % 4 == 3)
                s += "\n";
        }

        return s;
    }

private:
    uint8_t registers[32];
};

enum AvrRegisterClasses
{
    ALL_REGISTERS,
    REGISTERS_16PLUS,
    REGISTER0,
    REGISTER1
};

class AVRRegisterSlot : public RegisterSlot
{
public:
    AVRRegisterSlot(AvrRegisterClasses rclass, bool _write=false, bool _read=true) : RegisterSlot(_write, _read)
    {
        switch(rclass)
        {
        case ALL_REGISTERS:
            setValidArguments({0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31});
            break;
        case REGISTERS_16PLUS:
            setValidArguments({16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31});
            break;
        case REGISTER0:
            setValidArguments({0});
            break;
        case REGISTER1:
            setValidArguments({1});
            break;
        }
    }
};


 #include "avr_gen.hpp"

#endif
