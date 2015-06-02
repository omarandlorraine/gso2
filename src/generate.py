"""Generate the instruction classes

Usage:
    ./generate.py INPUTFILE OUTPUTFILE
"""

from docopt import docopt
import yaml

arguments = docopt(__doc__)

template = """
class Avr_{insn_name} : public Instruction
{{
public:
    unsigned execute(TargetMachine *_mach, Slot** slots)
    {{
        // TODO assert enough slots
        // TODO assert slot types are RegisterSlots

        AvrMachine *mach = static_cast<AvrMachine*>(_mach);
        {reg_read}

        {code}

        {reg_write}

        return {n_slots};
    }}

    // Return the number and types of register slots required
    //    which ones are read and written
    std::vector<Slot*> getSlots()
    {{
        return {{ {slots} }};
    }}

    std::string toString()
    {{
        std::string s = "{print_name} ";

        for(int i = 0; i < {n_slots}; ++i)
        {{
            s += "S" + std::to_string(i+1);
            if(i != {n_slots}-1)
                s += ", ";
        }}
        return s;
    }}

    std::string toString(Slot** slots)
    {{
        std::string s = "{print_name} ";

        for(int i = 0; i < {n_slots}; ++i)
        {{
            s += "r" + std::to_string(slots[i]->getValue());
            if(i != {n_slots}-1)
                s += ", ";
        }}
        return s;
    }}
}};
"""

with open(arguments["INPUTFILE"]) as f:
    obj = yaml.load(f)

with open(arguments["OUTPUTFILE"], "w") as fout:
    for insn_name, params in obj.items():
        code = params['implementation']
        operands = params['operands']

        print_name = params['print_name']
        reg_read = ""
        reg_write = ""
        slots = []
        for i, op in enumerate(operands):
            if "r" in op[0]:
                reg_read += "unsigned char r{} = mach->getRegister(slots[{}]);\n".format(chr(65+i), i)
            else:
                reg_read += "unsigned char r{} = 0;\n".format(chr(65+i))

            if "w" in op[0]:
                reg_write += "mach->setRegister(slots[{}], r{});\n".format(i, chr(65+i))

            slots.append("new AVRRegisterSlot(AvrRegisterClasses::{}, {}, {})".format(
                op[1],
                "true" if "w" in op[0] else "false",
                "true" if "r" in op[0] else "false"))
        slots = ", ".join(slots)
        n_slots = len(operands)

        s = template.format(**locals())

        fout.write(s)

