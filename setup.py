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

# UNIGE, DIFI, C03;
# Mattia Sotgia;

# TODO: 
# make macOs app that imput name of folder and title of article as string <-!!

import os, sys
import logging

from datetime import date

# base support for logging information;
logging.basicConfig(filename='setup.log', filemode='a', 
                    format='%(asctime)s %(levelname)s: %(message)s', datefmt='%m/%d/%Y %H:%M:%S',
                    level=logging.DEBUG)

NOW = date.today()
BASE_PATH = os.getcwd()
PATHS = [
    '/fig',
    '/relazione',
    '/dati',
    '/analisi_dati',
    '/misc'
]
LOG_MSG = {
    'path_created': 'Path {} created successfully',
    'path_failed': 'Creation of directory {} failed',
    'io_err': 'Unable to open/create requested file!',
    'path_exist': 'Path {} already exists! Please change dir name.\n',
    'path_empthy': 'Name for main dir for Lab. Experience is empthy!\n'
}
TEMP_FILE = open('template.tex')
README_STRING = '''README file
===========

Folder structure
----------------
main folder `/{}`

> `/relazione`
    for all pdf versions of paper
    also for all .tex and .docx eventually

> `/fig`
    for everythong concerning .figs, .pdfs, .ai's

> `/dati`
    raw and polished data goes here.

> `/analisi_dati`
    everuthing __code__ goes here.

> `/misc`
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

def LOGO_PRINT(): 
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

    c = 0
    
    while True:
        if len(sys.argv)<2+c:
            # print('Better compile as: python3 {} <esp_no> \n'.format(sys.argv[0]))
            arg_1 = input('Enter exp. `title_underscore`: ')
        else: arg_1 = sys.argv[1]
        c = 1        
        
        title_underscore = 'esperienza_{}'.format(arg_1)
        folder_path = BASE_PATH + '/' + title_underscore

        if os.path.exists(folder_path): print(LOG_MSG['path_exist'].format(title_underscore)); continue
        if arg_1 is '': print(LOG_MSG['path_empthy']); continue

        try: os.mkdir(folder_path)
        except OSError:
            logging.exception(LOG_MSG['path_failed'].format(folder_path))
        else:
            logging.info(LOG_MSG['path_created'].format(folder_path))
        break

    exp_no = arg_1[0:arg_1.find('_')]

    os.chdir(folder_path)

    readme_file = open('README.md', 'w')
    print(README_STRING.format(title_underscore), file=readme_file)


    print('Setting up directories...\n')
    for i in PATHS:
        try: os.mkdir(folder_path + i)
        except OSError:
            logging.exception(LOG_MSG['path_failed'].format(folder_path + i))
        else:
            logging.info(LOG_MSG['path_created'].format(folder_path + i))

    os.chdir(folder_path + PATHS[1])

    title_full = input('Enter full paper title Latex: ')
    try: latex_file = open('esperienza_{}_{}'.format(exp_no, NOW.strftime('%Y_%m_%d')) + '.tex', 'w')
    except IOError: print(LOG_MSG['io_err'])

    latex_readlines = TEMP_FILE.readlines()

    for line in latex_readlines:
        if '%%TITLE_HERE%%' in line:
            line = line.replace('%%TITLE_HERE%%', title_full)
        elif '%%DATE_HERE%%' in line:
            line = line.replace('%%DATE_HERE%%', NOW.strftime('%d %B %Y'))
        elif '%%NN%%' in line:
            line = line.replace('%%NN%%', exp_no)
        
        latex_file.write(line)

    print('Created file {} with paper title: {}\n'.format(latex_file.name, title_full))
    logging.info('Done, created {} file in {}'.format(latex_file.name, title_underscore + PATHS[1]))
    print('Done, see log file for errors!')