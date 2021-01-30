# setup.py, created 2021-01-20
# modified on 2021-01-25

# Change log:
# added some minor feature, 
# added support for full dir name

# UNIGE, DIFI, C03;
# Mattia Sotgia;

# TODO: 
# make macOs app that imput name of folder and title of article as string <-!!
import os, sys
import logging

from datetime import date

# base support for logging information;
logging.basicConfig(filename='setup.log', filemode='w', 
                    format='%(asctime)s %(levelname)s: %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p',
                    level=logging.DEBUG)

now = date.today()
current_path = os.getcwd()


if __name__ == "__main__":

    message = 'Compile as: python3 {} <esp_no>'.format(sys.argv[0])
    if len(sys.argv)<2:
        print(message)
        sys.exit(0)

    else:
        first_arg = sys.argv[1]
        esp_no = first_arg[0:first_arg.find('_')] 
        title_underscore = 'esperienza_{}'.format(first_arg)

        folder_full_path = current_path + '/' + title_underscore

try:
    os.mkdir(folder_full_path)
except OSError:
    logging.exception('Creation of directory {} failed'.format(folder_full_path))
else:
    logging.info('Path {} created successfully'.format(folder_full_path))
    
os.chdir(folder_full_path)

readme_file = open('README.md', 'w')
print('''README file
===========

Folder structure
----------------
main folder /{}

> /relazione
    for all pdf versions of paper
> /fig
    for everythong concerning .figs, .pdfs, .ai's
> /dati
    raw and polished data goes here.
> /analisi_dati
    everuthing __code__ goes here.
> /MOVETO_{}
    for everything that goes on the second machine 
    for latex typesetting
    > /fig
        a carbon copy of every fig in folder {}/fig 
    > /misc
        everything else (log files, results to be 
        copied)

'''.format(title_underscore, title_underscore, title_underscore), file=readme_file)

paths = [
   '/fig',
   '/relazione',
   '/dati',
    '/analisi_dati',
    '/MOVETO_{}'.format(title_underscore)
]

for i in paths:
    try:
        os.mkdir(folder_full_path + i)
    except OSError:
        logging.exception('Creation of directory {} failed'.format(folder_full_path + i))
    else:
        logging.info('Path {} created successfully'.format(folder_full_path + i))


movepath = folder_full_path + paths[4]
tex_path = [
'/fig',
    '/_misc'
]

for t in tex_path:
    try:
        os.mkdir(movepath + t)
    except OSError:
        logging.exception('Creation of directory {} failed'.format(movepath + t))
    else:
        logging.info('Path {} created successfully'.format(movepath + t))
        print('All path created successfully!')
        

os.chdir(movepath)


title_full = input('Enter full title Latex: ')


template_file = open('esperienza_{}_{}'.format(esp_no, now.strftime('%Y_%m_%d')) + '.tex', 'w')


tex_template = '''\\documentclass[italian, a4paper, 10pt, twocolumn]{{../style/lab_unige_v2}}
\\usepackage[a4paper, margin=1.25cm, footskip=0.25in]{{geometry}}

\\usepackage[utf8]{{inputenc}}
\\usepackage[T1]{{fontenc}}

\\usepackage[italian]{{babel}}

% \\usepackage{{biblatex}}

\\usepackage[bookmarksopen=true, 
citebordercolor={{0 1 0}}, 
linkbordercolor={{1 0 0}}, 
urlbordercolor={{0 1 1}}]{{hyperref}}
\\usepackage[numbered]{{bookmark}}

\\usepackage{{graphicx}}
\\usepackage{{array}}
\\usepackage{{tabulary}}
\\usepackage{{booktabs}}

% FOUNDAMENTAL
\\usepackage{{../style/custom}}

\\usepackage{{physics}}

\\usepackage{{breqn}}
\\usepackage{{cuted}}
\\usepackage{{txfonts}}

\\usepackage{{lipsum}}

%% Define ref types
\\newcommand{{\\reftab}}[1]{{Tab. {{\\ref{{#1}}}}}}%
\\newcommand{{\\reffig}}[1]{{Fig. {{\\ref{{#1}}}}}}%
\\newcommand{{\\refeqn}}[1]{{({{\\ref{{#1}})}}}}%
%%
\\setlength{{\\columnsep}}{{6mm}}
\\begin{{document}}


\\twocolumn[
  \\begin{{@twocolumnfalse}}
    
    \\title{{
        {}
    }}
    \\author{{
      Eugenio Dormicchi\\textsuperscript{{1}},  
      Riccardo Pizzimbone\\textsuperscript{{1}}, 
      Mattia Sotgia\\textsuperscript{{1}}
    }}

    \\date{{
      \\textsuperscript{{1}}Gruppo C03, Esperienza di laboratorio n. {}\\\\
      %\\textsuperscript{{2}}In presenza in laboratorio per la presa dati\\\\
    	% Università degli Studi di Genova, Dipartimento di Fisica.\\\\
    	Presa dati–– 
    	{}, 15:00– 18:00; Analisi dati–– 
    	<end-date here>
    }}
    \\maketitle
    
    \\begin{{abstract}}
      
      \\textit{{Obiettivo– }}
      
      \\textit{{Metodi– }}
      
      \\textit{{Risultati– }}
      
      \\textit{{Conclusione– }}
      
      
    \\end{{abstract}}
    \\vspace{{2em}}
    \\end{{@twocolumnfalse}}
]

  %%%% CORPO DEL TESTO
  %%%% CORPO DEL TESTO

  \\section{{Obiettivo}}
  \\label{{section:aim}}

  \\section{{Strumentazione}}
  \\label{{section:strument}}

  \\section{{Metodi}}
  \\label{{section:methods}}

  \\section{{Risultati}}
  \\label{{section:results}}

  \\section{{Conclusione}}
  \\label{{section:conclusion}}

  \\subsection{{Controlli}}

  \\subsection{{Possibili errori sistematici}}
  

\\end{{document}}
'''.format(title_full, esp_no, now.strftime('%d %B %Y'))

print(tex_template, file=template_file)

logging.info('Done')
print('Done, see log file for errors!')

# end of document
