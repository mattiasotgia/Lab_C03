import os

def set_folder(CWD_FOLDER):
    global MISC_FOLDER
    MISC_FOLDER = os.path.join(CWD_FOLDER, "../../misc")

__OUTPUT_FILE = None


def open_table_file(file_name):
    """
    """
    global __OUTPUT_FILE
    file_path = os.path.join(MISC_FOLDER, file_name)
    print('Opening output file %s...' % file_path)
    __OUTPUT_FILE = open(file_path, 'w')
    return __OUTPUT_FILE

def close_current_file():
    """
    """
    __OUTPUT_FILE.close()
    print('Current file closed.')

def format_number(val, precision=5, threshold=0.01):
    """
    """
    if isinstance(val, str):
        return val
    fmt = '%%.%df' % precision
    if val == 0:
        return '0'
    if abs(val) < threshold:
        val = '%e' % val
        mant, exp = str(val).split('e')
        mant = fmt % float(mant)
        exp = int(exp)
        return '%s \\times 10^{%d}' % (mant, exp)
    else:
        return fmt % val

def write_table_row(data, fmt=None):
    """
    """
    if fmt is None:
        fmt = '%s'
    if isinstance(fmt, str):
        fmt = [fmt]*len(data)
    line = ''
    for i, val in enumerate(data):
        line += '$%s$ & ' % (fmt[i] % val)
    line = line.strip('& ')
    line += '\\\\\n'
    __OUTPUT_FILE.write(line)

