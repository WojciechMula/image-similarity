import sys

from generate_base import ApplicationBase
from patterns import *


def main():
    path    = sys.argv[1]
    srcpath = sys.argv[2]

    app = Application(path, srcpath)
    app.run()


class Application(ApplicationBase):
    def __init__(self, path, srcpath):
        super(Application, self).__init__(path)
        self.srcpath = srcpath


    def run(self):
        self.load()
        self.simplify()

        self.generate()


    def generate(self):
        program = []
        for (value, type, params) in self.pick_algorithm():
            if type == 'null':
                program.append(('', ''))
            elif type == 'pshufb':
                program.append(self.generate_pshufbased(value))
            elif type == 'scalar':
                program.append(self.generate_scalar(value))
            elif type == 'algorithm1':
                scale, shift = params
                program.append(self.generate_procedure(value, scale, shift,
                               self.algorithm1_pattern,
                               ALGORITHM1_PATTERN_GENERIC,
                               ALGORITHM1_PATTERN_NO_SHIFT))
            elif type == 'algorithm2':
                scale, shift = params
                program.append(self.generate_procedure(value, scale, shift,
                               self.algorithm2_pattern,
                               ALGORITHM2_PATTERN_GENERIC,
                               ALGORITHM2_PATTERN_NO_SHIFT))
            else:
                assert False


        self.write_program(program)


    def write_program(self, program):
        with open(self.srcpath, 'wt') as f:
            def writeln(s):
                f.write(s)
                f.write('\n')

            write = f.write

            for name, code in program:
                if name:
                    writeln(DECLARATION % {'name': name})

            writeln('typedef void (*normalize_values_inplace_fn)(uint8_t, uint8_t*, size_t);')
            writeln('normalize_values_inplace_fn normalize_function_lookup[256] = {')
            for i, (name, code) in enumerate(program):
                write('\t')
                write(name or 'NULL')
                if i < 255:
                    write(',')
                write('\n')

            writeln('};')

            for name, code in program:
                writeln(code)


    def pick_algorithm(self):

        self.simplify()
        for value in xrange(0, 256):
            if value in self.algorithm1 and value in self.algorithm2:
                scale1, shift1 = self.algorithm1[value][0]
                scale2, shift2 = self.algorithm2[value][0]
                
                if shift2 == 0:
                    yield (value, 'algorithm1', (scale2, shift2))
                else:
                    yield (value, 'algorithm2', (scale1, shift1))

            elif value in self.algorithm1:
                yield (value, 'algorithm1', self.algorithm1[value][0])
            elif value in self.algorithm2:
                yield (value, 'algorithm2', self.algorithm2[value][0])
            elif 1 <= value <= 15:
                yield (value, 'pshufb', None)
            elif value in [0, 255]:
                yield (value, 'null', None)
            else:
                yield (value, 'scalar', None)

    def simplify(self):
        super(Application, self).simplify(self.algorithm1)
        super(Application, self).simplify(self.algorithm2)

main()
