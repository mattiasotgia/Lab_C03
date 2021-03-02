# setup.py, created 2021-01-20
# modified on 2021-01-25

# Change log:
# added some minor feature, 
# added support for full dir name
# 2021-02-03 moved everything in one single 
# dir, as latex file go into ../relazione/
# 2021-02-05 reformatted README.md for every
#  single istance of lab
# 2021-02-11 rewritten setup.py for latex 
# template, so it is easier to modify master 
# template file
# 2021-02-13 created init costants strings 
# and dict for given values. Updated input 
# method to have also recursive support.
# 2021-02-15 updated input method with error 
# handling, added auto numbering
# 2021-02-17 removed numpy unnecessary import,
# resolved max([]) at line 132-134
# 2021-02-25 resolved minor fixes, made 
# colored output, minor fixes on OSError, 
# set sys.exit(0)

# UNIGE, DIFI, C03;
# Mattia Sotgia;

# TODO:
# !!! make program chdir in terminal automatically at progrem end !!!
# make macOs app that imput name of folder and title of article as string <-!!

import os, sys
import logging
import subprocess

from datetime import date

# base support for logging information;
logging.basicConfig(filename='setup.log', filemode='a', 
                    format='%(asctime)s %(levelname)s: %(message)s', datefmt='%m/%d/%Y %H:%M:%S',
                    level=logging.DEBUG)

NOW = date.today()
BASE_PATH = os.getcwd()
BASE_DIR_NAME = 'esperienza_'
ALL_EXP_N = list(int(n[len(BASE_DIR_NAME):n.find('_', len(BASE_DIR_NAME))]) \
    for n in os.listdir(BASE_PATH) if n[:len(BASE_DIR_NAME)] == BASE_DIR_NAME)
PATHS = [
    '/fig',
    '/relazione',
    '/dati',
    '/analisi_dati',
    '/misc',
]
LOG_MSG = {
    'path_created': 'Path {} created successfully',
    'path_failed': 'Creation of directory {} failed',
    'io_err': 'Unable to open/create requested file!',
    'path_exist': '\033[1;31mPath {} already exists! Please change dir name.\n\033[0m',
    'path_empthy': '\033[1;31mName for main dir for Lab. Experience is empthy!\n\033[0m',
    'not_exp_n': '\033[31mDir name should start with Lab. Experience NUMBER!\nAdding auto Lab. Exp. N* -> \033[0m',
    'exist_exp_number': '\033[31mPath esperienza_{}... exist, moving -> esperienza_{}...\033[0m',
    'i_dir_name': 'Enter exp. `title_underscore` 📁 (`exit` to quit): ',
    'i_paper_title': '\nEnter full paper title Latex 📝 : ',
    'u_quit': '\033[1;31mUser quit...\033[0m',
    'quit_delete': '\033[31mDo you want to delete {} directory? ( y / n )\033[0m',
}
SYS_EXIT = [
    '.q',
    'quit()',
    'exit',
    'q',
    '\x1b',
]
TEMP_FILE = open('template.tex')
README_STRING = '''README file
===========
{doc}

Folder structure
----------------
main folder `/{path}`

* `/relazione`
    for all pdf versions of paper
    also for all .tex and .docx eventually

* `/fig`
    for everythong concerning .figs, .pdfs, .ai's

* `/dati`
    raw and polished data goes here.

* `/analisi_dati`
    everything __code__ goes here.

* `/misc`
    everything else (log files, results to be 
    copied)

'''
LOGO = [
'                                                            ',
'                                                            ',
'   **                 **             ******   ****   ****   ',
'  /**                /**            **////** *///** */// *  ',
'  /**        ******  /**           **    // /*  */*/    /*  ',
'  /**       ´´´´´´** /******      /**       /* * /*   ***   ',
'  /**        ******* /**///**     /**       /**  /*  /// *  ',
'  /**       **´´´´** /**  /** **  //**    **/*   /* *   /*  ',
'  /********//********/****** /**   //****** / **** / ****   ',
'  ////////  //////// /////   //     //////   ////   /´///   ',
'                                                            ',
'                                                            ',
]

def LOGO_PRINT() -> None: 
        for line in LOGO: print(line)

if __name__ == "__main__":
    '''setup.py python3 file
    This file is made to be used as a light tools for directory managment in Lab1A papers

    Usage:
        - the main program is run by command line as python3 setup.py 'name of experience' 
            - the name of experience should be formatted as NN_<brief_title_with_underscores>
        - there are no dependencies other than os, sys, datetime and logging
    
    '''

    LOGO_PRINT() # Print Lab. C03 intro page

    os.chdir(BASE_PATH)

    
    while True:
        if len(sys.argv)<2 or sys.argv[1] is None:
            arg_1 = input(LOG_MSG['i_dir_name'])
        else: arg_1 = sys.argv[1]; sys.argv[1] = None

        arg_1 = arg_1.replace(' ', '_')
        
        if arg_1 in SYS_EXIT: print(LOG_MSG['u_quit']); sys.exit(0)

        if arg_1 == '': print(LOG_MSG['path_empthy']); continue
        
        exp_no = arg_1[:arg_1.find('_')]

        if not arg_1[0].isdigit():
            if arg_1[0] != '_': arg_1 = '_' + arg_1
            exp_no = str(max(ALL_EXP_N)+1); print(LOG_MSG['not_exp_n'] + exp_no);
        elif int(exp_no) <= max(ALL_EXP_N):
            exp_no = str(max(ALL_EXP_N)+1)
            print(LOG_MSG['exist_exp_number'].format(str(int(exp_no) - 1), exp_no))

        title_underscore = BASE_DIR_NAME + exp_no + '_' + arg_1[arg_1.find('_'):]
        folder_path = BASE_PATH + '/' + title_underscore

        if os.path.exists(folder_path): print(LOG_MSG['path_exist'].format(title_underscore)); continue # Check if full directory exists

        break

    try: os.mkdir(folder_path)
    except OSError:
        logging.exception(LOG_MSG['path_failed'].format(folder_path))
        sys.exit(0)
    else:
        logging.info(LOG_MSG['path_created'].format(folder_path))

    os.chdir(folder_path)

    title_full = input(LOG_MSG['i_paper_title'])


    with open('README.md', 'w') as readme_file:
        print(README_STRING.format(doc=title_full, path=title_underscore), file=readme_file)


    print('\033[0;32mSetting up directories...\n\033[0m')
    for i in PATHS:
        try: os.mkdir(folder_path + i)
        except OSError:
            logging.exception(LOG_MSG['path_failed'].format(folder_path + i))
            sys.exit(0)
        else:
            logging.info(LOG_MSG['path_created'].format(folder_path + i))

    os.chdir(folder_path + PATHS[1])

    
    if title_full in SYS_EXIT:
        ask = input(LOG_MSG['quit_delete'].format(title_underscore))
        if ask in ['y', 'Y', 'yes', 'Yes', 'YES']:
            subprocess.run(['rm', '-r', BASE_PATH + '/' + title_underscore])
        print(LOG_MSG['u_quit']); sys.exit(0)

    try: latex_file = open('esperienza_{}_{}'.format(exp_no, NOW.strftime('%Y_%m_%d')) + '.tex', 'w')
    except IOError: print(LOG_MSG['io_err'])

    
    latex_readlines = TEMP_FILE.readlines()

    for line in latex_readlines:
        if '%%TITLE_HERE%%' in line: line = line.replace('%%TITLE_HERE%%', title_full)
        if '%%DATE_HERE%%' in line: line = line.replace('%%DATE_HERE%%', NOW.strftime('%d %B %Y'))
        if '%%NN%%' in line: line = line.replace('%%NN%%', exp_no)
        
        latex_file.write(line)

    print('\n📄 Created file {} with paper title: {}\n'.format(latex_file.name, title_full))
    logging.info('Done, created {} file in {}'.format(latex_file.name, title_underscore + PATHS[1]))
    print('\033[1;32mDone, see log file for errors!\n\033[1;33mMove to ./{}\033[0m'.format(title_underscore))
    # TODO: add command to make python cd to ./esperienza_#_<<>> <-- might not be possible