label_define = '::'
label_ref = '``'
def_prefix = 'def'
def_ref_prefix = '$'
hex_prefix = '0x'
bin_prefix = '%'
str_prefix = '&'
data_prefix = '='

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
    Opcode('jump', 4, lambda x:x == 2, None, None),
    Opcode('return', 5, None, None, None),
    Opcode('goto', 6, lambda x:x == 1, None, None),
    Opcode('halt', 7, None, None, None),
    Opcode('compare', 8, lambda x:x == 2, None, None),
    Opcode('branch', 9, lambda x:x == 3, None, None),
    Opcode('mult', 10, lambda x:x == 3, None, None),
    Opcode('div', 11, lambda x:x == 3, None, None),
    Opcode('sub', 12, lambda x:x == 3, None, None),
    Opcode('pack8', 13, lambda x:x == 5, None, None),
    Opcode('unpack8', 14, lambda x:x == 2, None, None),
    Opcode('pack16', 15, lambda x:x == 3, None, None),
    Opcode('unpack16', 16, lambda x:x == 2, None, None),
    Opcode('clear', 17, None, None, None),
    Opcode('memset', 18, lambda x:x == 3, None, None),
    Opcode('memcpy', 19, lambda x:x == 3, None, None),
    Opcode('setpix', 20, lambda x:x == 4, None, None),
    Opcode('drawbmap', 21, lambda x:x == 6, None, None),
    Opcode('drawbmap_p', 22, lambda x:x == 7, None, None),
    Opcode('rand', 23, lambda x:x == 1, None, None),
    Opcode('dsync', 24, lambda x:x == 0, None, None)
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
        self.length = int_read(segments[1])
        self.arg1 = int_read(segments[2])
        self.arg2 = int_read(segments[3])
        if self.length > 255 or self.arg1 > 255 or self.arg2 > 255:
            raise Exception('Length or args are > 255. Length:{} Arg1:{} Arg2:{}'.format(self.length, self.arg1, self.arg2))
        if self.length > 0:
            self.payload = list()
            for n in segments[4:self.length + 4]:
                if n.startswith(label_ref):
                    label_name = n.split(label_ref)[1].rstrip()
                    try:
                        label = [l for l in prog.labels if l.name == label_name][0]
                    except:
                        raise Exception('Reference to label {} encountered but label not found'.format(label_name))
                    lr = LabelReference(label)
                    self.payload.append(lr)
                else:
                    self.payload.append(int_read(n))
        if opcode.validate(self):
            pass
        else:
            raise Exception('Instruction did not pass validation for {}\nInstruction string: {}'.format(self.op_name, self.instruction_string))

    def out(self):
        print('Outputting binary for op {} ({}) len {} arg1 {} arg2 {} payload {}'.format(self.op_number, self.op_name, self.length, self.arg1, self.arg2, self.payload))
        instruction_int = self.op_number
        instruction_int += self.length * 2 ** 8
        instruction_int += self.arg1 * 2 ** 16
        instruction_int += self.arg2 * 2 ** 24
        result = list()
        result.append(instruction_int)
        if self.payload is not None:
            result += self.payload
        return b''.join([n.to_bytes(4, byteorder='little') for n in result])


class Label(object):
    def __init__(self, name):
        self.name = name
        self.value = None


class LabelReference(object):
    def __init__(self, label):
        self.label = label


class Define(object):
    def __init__(self, name, value):
        self.name = name
        self.value = int_read(value)


class DefineReference(object):
    def __init__(self, define):
        self.define = define


class Data(object):
    def __init__(self, value):
        self.value = value
    
    def out(self):
        print('Outputting binary for data {}'.format(self.value))
        return self.value.to_bytes(4, byteorder='little')


class Program(object):
    def __init__(self, start_address=1000000):
        self.program = list()
        self.labels = list()
        self.defs = list()
        self.start_addres = start_address

    def resolve_labels(self):
        position = self.start_addres
        # Pass 1 - set values of all labels
        for step in self.program:
            if isinstance(step, Label):
                print('Label {} has value {}'.format(step.name, position))
                step.value = position
            elif isinstance(step, Instruction):
                position += step.length + 1
            elif isinstance(step, Data):
                position += 1
            else:
                raise Exception('Unknown item in program: {}'.format(step))
        # Pass 2 - update references
        for step in self.program:
            if isinstance(step, Instruction):
                if step.length > 0:
                    for i in range(0, step.length):
                        if isinstance(step.payload[i], LabelReference):
                            print('Found ref to label {}'.format(step.payload[i].label.name))
                            step.payload[i] = step.payload[i].label.value

    def out(self):
        inst = [i for i in self.program if isinstance(i, Instruction) or isinstance(i, Data)]
        return inst

    



def read_asm(data):
    lines = [l.lstrip().split('#')[0] for l in data.split('\n') if l != '' and l.split('#')[0] != '']
    # find all labels pass
    prog.labels = [Label(line.split(label_define)[1].rstrip()) for line in lines if line.startswith(label_define)]
    # find all defs pass
    prog.defs = [Define(line.split(def_prefix)[1].rstrip().split()[0], line.split(def_prefix)[1].rstrip().split()[1]) for line in lines if line.startswith(def_prefix)]
    # scan through, create series of labels and instructions
    for line in lines:
        if line.startswith(label_define):
            l = [label for label in prog.labels if label.name == line.split(label_define)[1].rstrip()][0]
            prog.program.append(l)
        elif line.startswith(def_prefix):
            pass
        elif line.startswith(data_prefix):
            data_part = line.split(data_prefix, 1)[1]
            if data_part.startswith(str_prefix):
                str_data = data_part.split(str_prefix, 1)[1]
                i = 0
                while i < len(str_data):
                    c = str_data[i]
                    if c == '~':
                        nc = str_data[i+1]
                        if nc == 't':
                            d = Data(ord('\t'))
                        elif nc == 'n':
                            d = Data(ord('\n'))
                        else:
                            d = Data(ord(nc))
                        prog.program.append(d)
                        i += 2
                    else:
                        d = Data(ord(c))
                        prog.program.append(d)
                        i += 1
            else:
                d = Data(int_read(line.split(data_prefix)[1].rstrip()))
                prog.program.append(d)
        else:
            i = Instruction(line)
            prog.program.append(i)


def write_bin(file_name, instructions):
    with open(file_name, 'wb') as f:
        f.write(b''.join([i.out() for i in instructions]))


def int_read(int_str):
    if int_str.startswith(hex_prefix):
        return int(int_str, 0)
    elif int_str.startswith(bin_prefix):
        return int(int_str.split(bin_prefix)[1], 2)
    elif int_str.startswith(def_ref_prefix):
        try:
            my_def = [d for d in prog.defs if d.name == int_str.split(def_ref_prefix)[1]][0]
            return my_def.value
        except:
            raise Exception('Define {} not found'.format(int_str))
    else:
        return int(int_str)


if __name__=='__main__':
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', default='./test.asm')
    parser.add_argument('-o', default='../test.bin')
    parser.add_argument('-d', default='./std_defs.asm')
    parser.add_argument('-s', default=1000000)
    args = parser.parse_args()
    #inst = read_asm(args.i)
    #write_bin(args.o, inst)
    prog = Program(start_address=int(args.s))

    
    with open(args.d, 'r') as f:
        asm = f.read()
    
    with open(args.i, 'r') as f:
        asm += f.read()

    read_asm(asm)
    prog.resolve_labels()
    inst = prog.out()
    write_bin(args.o, inst)
