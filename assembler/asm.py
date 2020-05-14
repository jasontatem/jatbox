label_sep = '::'

class Opcode(object):
    def __init__(self, op_name, op_number, length_condition, arg1_condition, arg2_condition):
        self.op_name = op_name
        self.op_number = op_number
        self.length_condition = length_condition
        self.arg1_condition = arg1_condition
        self.arg2_condition = arg2_condition

    def validate(self, instruction):
        if self.length_condition is not None and not self.length_condition(instruction.length):
            print('Length {} does not pass condition for {}'.format(instruction.length, self.op_name))
            return False
        if instruction.payload is not None and instruction.length != len(instruction.payload):
            print('Length {} specified but found {} values'.format(instruction.length, len(instruction.payload)))
            return False
        if self.arg1_condition is not None and not self.arg1_condition(instruction.arg1):
            print('Arg1 condition not met by {}'.format(instruction.arg1))
            return False
        if self.arg2_condition is not None and not self.arg2_condition(instruction.arg1):
            print('Arg2 condition not met by {}'.format(instruction.arg2))
            return False
        return True



opcodes = [
    Opcode('noop', 0, lambda x:x == 0, None, None),
    Opcode('store', 1, lambda x:x == 2, None, None),
    Opcode('copy', 2, lambda x:x == 2, None, None),
    Opcode('add', 3, lambda x:x == 3, None, None),
    Opcode('jump', 4, lambda x:x == 1, None, None),
    Opcode('return', 5, None, None, None),
    Opcode('goto', 6, lambda x:x == 1, None, None),
    Opcode('halt', 7, None, None, None),
    Opcode('compare', 8, lambda x:x == 2, None, None),
    Opcode('branch', 9, lambda x:x == 2, None, None),
    Opcode('mult', 10, lambda x:x == 3, None, None)
]


class Instruction(object):
    def __init__(self, instruction_string):
        self.instruction_string = instruction_string
        self.op_name = None
        self.op_number = None
        self.length = None
        self.arg1 = None
        self.arg2 = None
        self.payload = None
        self.parse()

    def parse(self):
        segments = self.instruction_string.split()
        self.op_name = segments[0]
        try:
            opcode = [op for op in opcodes if op.op_name == self.op_name.lower()][0]
        except:
            raise Exception('Instruction name {} not found in opcode list'.format(self.op_name))
        self.op_number = opcode.op_number
        self.length = int(segments[1])
        self.arg1 = int(segments[2])
        self.arg2 = int(segments[3])
        if self.length > 0:
            self.payload = [int(n) for n in segments[4:self.length + 4]]
        if opcode.validate(self):
            pass
        else:
            raise Exception('Instruction did not pass validation for {}'.format(self.op_name))

    def out(self):
        instruction_int = self.op_number
        instruction_int += self.length * 2 ** 8
        instruction_int += self.arg1 * 2 ** 16
        instruction_int += self.arg1 * 2 ** 24
        result = list()
        result.append(instruction_int)
        if self.payload is not None:
            result += self.payload
        return b''.join([n.to_bytes(4, byteorder='little') for n in result])


def read_asm(file_name):
    with open(file_name, 'r') as f:
        data = f.read()
        lines = [l.split('#')[0] for l in data.split('\n')]
        instructions = [Instruction(line) for line in lines if line != '']
    return instructions


def write_bin(file_name, instructions):
    with open(file_name, 'wb') as f:
        f.write(b''.join([i.out() for i in instructions]))

if __name__=='__main__':
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', default='./test.asm')
    parser.add_argument('-o', default='../test.bin')
    args = parser.parse_args()
    inst = read_asm(args.i)
    write_bin(args.o, inst)
