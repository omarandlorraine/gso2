#ifndef __INSTRUCTION_HPP__
#define __INSTRUCTION_HPP__

#include <vector>
#include "slots.hpp"
#include "utility.hpp"

class Slot;
class TargetMachineBase;
class Backend;

/*! \class Instruction

    The Instruction base class is intended to be subclassed by instructions
    that the target processor provides. The execute function, and getSlots are
    the most critical, since they provide the actual implementation of the
    instruction. The other methods allow instructions to be printed and
    parsed.

    The entire instructions classes can be generated by the generate.py.
*/
class Instruction
{
public:
    virtual ~Instruction() {};

    /*! The execution function provides an implementation of the instruction,
        applying to the passed in machine. The slots are used to indicate
        which registers, and constants the instruction should use, etc. The
        function should return the number of slots used by the instruction.

        An example of the and instruction for AVR is given below.
        \code
            // Cast the given MachineTarget to the correct type
            AvrMachine *mach = static_cast<AvrMachine*>(_mach);

            // Retrieve register
            uint8_t rA = mach->getRegister(slots[0]);
            uint8_t rB = mach->getRegister(slots[1]);

            // Perform the operation, set flags
            rA = rA & rB;
            setFlagSVNZ(mach, rA, 0);

            // Set registers
            mach->setRegister(slots[0], rA);

            return 2;
        \endcode

        @param _mach    The machine state, which will be modified as per the
                        operation of the instruction.
        @param slots    The first slot to be used by the instruction. Normally
                        the list of slots would have been composed of the
                        result of getSlots, with the appropriate pointer to
                        the list passed in here.
        @return         The number of slots used by the instruction (same as
                        getNumberOfSlots).
    */
    virtual unsigned execute(TargetMachineBase *_mach, Slot **slots) = 0;

    /*! Return a list of slots used by the instruction.

        The function allocates the correct number and type of slots for the
        instruction. These are then owned by the callee, and must be
        deallocated.

        For example, a three operand add instruction would return three slots,
        each derived from RegisterSlot.

        @return  A list of slots which the instruction operates on.

    */
    virtual std::vector<Slot*> getSlots() {return {};}

    /*! Return the number of slots that the instruction operates on.

        @return  The number of slots that the instruction's execute method
                 requires.
    */
    virtual unsigned getNumberOfSlots() {return 0;}


    virtual std::string toString() { return ""; }

    /*! Represent the instruction as a string.

        E.g. "add r0, r1, r3", if the slots had the values 0, 1 and 3.

        @param slots   The slots the instruction requires.
        @return        A string with a textual representation of the
                       instruction.
    */
    virtual std::string toString(Slot **slots) { return "";}

    /*! Return a string with the instruction mnemonic.

        E.g. "add"

        @return  The instruction mnemonic.
    */
    virtual std::string getName() { return "";}


    /*! Construct a list of slots based on a string.

        The parse function attempts to use the instruction's format to
        determine whether a string matches the instruction. It also extracts
        values for each of the slots used by the instruction, if there is a
        match.

        For example, with the input "add r1, r3, r2", the instruction will
        attempt to match "add", then read three values into slots: 1, 3, 2. If
        the function could not parse the string, false is returned.

        @param input        The string to parse as an instruction.
        @param slotlist     The list to place the recorded slot values in.
        @return             Whether or not the instruction could be parsed,
                            and the slotlist contents are valid.
    */
    virtual bool parse(std::string input, std::vector<Slot*> &slotlist) { return false;};
};

#endif
