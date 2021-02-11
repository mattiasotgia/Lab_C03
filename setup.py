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
                    format='%(asctime)s %(levelname)s: %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p',
                    level=logging.DEBUG)

now = date.today()
base_path = os.getcwd()
temp_file = open('template.tex')
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


if __name__ == "__main__":
    '''setup.py python3 file
    This file is made to be used as a light tools for directory managment in Lab1A papers

    Usage:
        - the main program is run by command line as python3 setup.py 'name of experience' 
            - the name of experience should be formatted as NN_<brief_title_with_underscores>
        - there are no dependencies other than os, sys, datetime and logging
    
    '''

    if len(sys.argv)<2:
        print('Compile as: python3 {} <esp_no>'.format(sys.argv[0]))
        sys.exit(0)

    else:
        arg_1 = sys.argv[1]
        exp_no = arg_1[0:arg_1.find('_')] 
        title_underscore = 'esperienza_{}'.format(arg_1)

        folder_path = base_path + '/' + title_underscore

    try:
        os.mkdir(folder_path)
    except OSError:
        logging.exception('Creation of directory {} failed'.format(folder_path))
    else:
        logging.info('Path {} created successfully'.format(folder_path))
    
    os.chdir(folder_path)

    readme_file = open('README.md', 'w')
    print(README_STRING.format(title_underscore), file=readme_file)

    paths = [
        '/fig',
        '/relazione',
        '/dati',
        '/analisi_dati',
        '/misc'
    ]

    for i in paths:
        try:
            os.mkdir(folder_path + i)
        except OSError:
            logging.exception('Creation of directory {} failed'.format(folder_path + i))
        else:
            logging.info('Path {} created successfully'.format(folder_path + i))


    os.chdir(folder_path + paths[1])


    title_full = input('Enter full title Latex: ')

    latex_file = open('esperienza_{}_{}'.format(exp_no, now.strftime('%Y_%m_%d')) + '.tex', 'w')

    latex_readlines = temp_file.readlines()

    for line in latex_readlines:
        if '%%TITLE_HERE%%' in line:
            line = line.replace('%%TITLE_HERE%%', title_full)
        elif '%%DATE_HERE%%' in line:
            line = line.replace('%%DATE_HERE%%', now.strftime('%d %B %Y'))
        elif '%%NN%%' in line:
            line = line.replace('%%NN%%', exp_no)
        
        latex_file.write(line)

    print('Created file {} with paper title: {}'.format('esperienza_{}_{}'.format(exp_no, now.strftime('%Y_%m_%d')) + '.tex', title_full))
    logging.info('Done, created {} file in {}'.format('esperienza_{}_{}'.format(exp_no, now.strftime('%Y_%m_%d')) + '.tex', title_underscore + paths[1]))
    print('Done, see log file for errors!')

    # end of document