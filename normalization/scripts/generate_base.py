import sys

from patterns import *

class ApplicationBase(object):
    def __init__(self, path):
        self.path = path


    def load(self):
        self.algorithm1 = {}
        self.algorithm2 = {}
        with open(self.path) as f:
            self.load_from_file(f)


    def load_from_file(self, file):
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


    def generate_procedures(self, parameters, name_pattern, code_pattern_generic, code_pattern_no_shift):
        procedures = {}
        for value in sorted(parameters):
            scale, shift = parameters[value][0] # get the first
            procedures[value] = self.generate_procedure(value, scale, shift, name_pattern,
                                                        code_pattern_generic, code_pattern_no_shift)

        return procedures


    def generate_procedure(self, value, scale, shift, name_pattern, code_pattern_generic, code_pattern_no_shift):
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

        return (data['name'], code);


    def generate_pshufbased(self, value):
        assert 1 <= value < 16

        values = [0] * 16
        for i in xrange(value + 1):
            values[i] = int(i * (255.0 / value))

        values_fmt = ', '.join(('%d' % x) for x in values)
        data = {
            'name': 'normalize_pshufb_%d' % value,
            'lookup_values' : values_fmt,
        }

        return (data['name'], PSHUFB_ALGORITHM % data)


    def generate_scalar(self, value):

        data = {
            'name'  : ('normalize_scalar_%d' % value),
            'value' : ('%d' % value)
        }

        return (data['name'], SCALAR_ALGORITHM % data)
