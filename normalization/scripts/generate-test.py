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
        self.path    = path
        self.srcpath = srcpath


    def run(self):
        self.load()
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

        algo3 = {}
        for value in xrange(1, 16):
            algo3[value] = self.generate_pshufbased(value)

        algo4 = {}
        for value in xrange(1, 254):
            if value in algo1 or value in algo2 or value in algo3:
                continue

            algo4[value] = self.generate_scalar(value)


        with open(self.srcpath, 'wt') as f:
            def writeln(s):
                f.write(s)
                f.write('\n')

            write = f.write

            writeln('#include <stdint.h>')
            writeln('#include <stddef.h>')
            writeln('#include <immintrin.h>')

            def write_declarations(algorithms):
                for value in sorted(algorithms):
                    writeln(DECLARATION % {'name': algorithms[value][0]})

            def write_implementations(algorithms):
                for value in sorted(algorithms):
                    write(algorithms[value][1])

            # 1. declarations
            write_declarations(algo1);
            write_declarations(algo2);
            write_declarations(algo3);
            write_declarations(algo4);

            # 2. lookup
            writeln('typedef void (*normalize_values_inplace_fn)(uint8_t, uint8_t*, size_t);')

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
            write_lookup('algorithm3', algo3)
            write_lookup('algorithm4', algo4)

            # 3. implementations
            write_implementations(algo1)
            write_implementations(algo2)
            write_implementations(algo3)
            write_implementations(algo4)


if __name__ == '__main__':
    main()
