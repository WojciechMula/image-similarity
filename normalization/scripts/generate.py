import sys

from patterns import *

def main():
    path    = sys.argv[1]
    srcpath = sys.argv[2]

    app = Application(path, srcpath)
    app.run()


class Application():
    def __init__(self, path, srcpath):
        self.path    = path
        self.srcpath = srcpath
        self.algorithm1 = {}
        self.algorithm2 = {}


    def run(self):
        with open(self.path) as f:
            self.load(f)

        self.simplify(self.algorithm1)
        self.simplify(self.algorithm2)

        self.generate()


    def generate(self):
        algo1 = self.generate_procedures(
                    self.algorithm1,
                    'normalize1_%(value)s',
                    ALGORITHM1_PATTERN_GENERIC,
                    ALGORITHM1_PATTERN_NO_SHIFT)

        algo2 = self.generate_procedures(
                    self.algorithm2,
                    'normalize2_%(value)s',
                    ALGORITHM2_PATTERN_GENERIC,
                    ALGORITHM2_PATTERN_NO_SHIFT)

        with open(self.srcpath, 'wt') as f:
            def writeln(s):
                f.write(s)
                f.write('\n')

            write = f.write

            writeln('#include <stdint.h>')
            writeln('#include <stddef.h>')
            writeln('#include <immintrin.h>')

            # 1. declarations
            for value in sorted(algo1):
                writeln(DECLARATION % {'name': algo1[value][0]})

            for value in sorted(algo2):
                writeln(DECLARATION % {'name': algo2[value][0]})

            # 2. lookup
            writeln('typedef void (*normalize_values_inplace_fn)(uint8_t* data, size_t s);')

            def write_lookup(name, code):
                writeln('normalize_values_inplace_fn %s[256] = {' % name)
                for value in xrange(256):
                    write('\t')
                    try:
                        write(code[value][0])
                    except KeyError:
                        write('NULL')

                    if value < 255:
                        write(',\n')
                    else:
                        write('\n')

                writeln('};')

            write_lookup('algorithm1', algo1)
            write_lookup('algorithm2', algo2)

            # 3. implementations
            for value in sorted(algo1):
                write(algo1[value][1])

            for value in sorted(algo2):
                write(algo2[value][1])



    def load(self, file):

        for line in file:
            F = line.split()
            if F[0] == 'algorithm1':
                algorithm = self.algorithm1
            elif F[0] == 'algorithm2':
                algorithm = self.algorithm2
            else:
                assert False

            value = int(F[3])
            scale = int(F[6])
            shift = int(F[9])

            if value not in algorithm:
                algorithm[value] = []

            algorithm[value].append((scale, shift))


    def simplify(self, parameters):
        # If for given value we have shift = 0, then we obviously might
        # pick shorter code. In such case remove all other variants
        for value in parameters:
            params = parameters[value]

            def shorter_alternatrive():
                return any(True for scale, shift in params if shift == 0)

            if shorter_alternatrive():
                print "value = %d" % value
                parameters[value] = [(scale, 0) for scale, shift in params if shift == 0]


    def generate_procedures(self, parameters, name_pattern, code_pattern_generic, code_pattern_no_shift):
        procedures = {}
        for value in sorted(parameters):
            scale, shift = parameters[value][0] # get the first

            data = {}

            data['value'] = '%d' % value
            data['scale'] = '%d' % scale
            data['shift'] = '%d' % shift
            data['name']  = name_pattern % data

            if shift == 0:
                code_pattern = code_pattern_no_shift
            else:
                code_pattern = code_pattern_generic

            code = code_pattern % data

            procedures[value] = (data['name'], code);

        return procedures


main()
