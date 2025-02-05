# Lab C03 Group (2020-21) :rocket:

This repo is made to hold all the code and data generated by lab experiments.

![GitHub release (latest by date)](https://img.shields.io/github/v/release/mattiasotgia/Lab_C03?label=latest%20relase)

There are two branches: `master` and `working-on`. The latter is currently being 
used for new insertion, and active development, while the former is a sort of 
'archive' for past lab experiences.

***Latex files***
Current status of latex files.

[![Build LaTeX document](https://github.com/mattiasotgia/Lab_C03/actions/workflows/build_latex.yml/badge.svg)](https://github.com/mattiasotgia/Lab_C03/actions/workflows/build_latex.yml)


## Usage

This repo should be maintained up-to-date with the latest experiments and data.

There is a `./esperienza_#__<title>` folder for each one lab. experience. Those 
are then divided as described in README for each sub-directory. 

The `setup.py` file is meant to be used to create the base project for a lab. 
experience. To run [setup.py](setup.py) use `python3 setup.py <OPTIONAL: expNumber_title>`

## Requirements

Software needed to use this repo is listed below:

- [`root`](https://github.com/root-project/root), An open-source data analysis 
framework used by high energy physics and others, developed by CERN 
(currently using [`v6.22/06`](https://github.com/root-project/root/tree/v6-22-06) 
to run code);
- `stat` custom library (Currently `v1.0`). Run as `g++ -o <exe_name> <program.cpp> stat.cpp`.
- various python libraries are used: `numpy`, `matplotlib`, `scipy`, `pandas`, with different use-cases.
<!-- - `numpy` Python library is only needed in order for the [setup.py](setup.py) program to work.  -->

Future code might need other libraries, that will be eventually be written here.

## Stat Library

`stat` is a `C++` library made exclusively to compute the statistical average and
standard error. It is located in [`stat/stat.h`](stat/stat.h).

***Library Usage***

This library is just a piece of code written to facilitate some computing, eventually 
it needs to be implemented in the code.

* `stat::read_values()` takes a `stdin::vector` as input, whereas the vector contain all 
data needed in calculation.

* `stat::avg()` return average of elements in the `vector`.

* `stat::std_err()` return standard error computed from RMS (Root Minimum Square).

* `stat::clear()` is needed to clean up the data vector, in order to compute other data. 

Changelog
---------
**2021-02-01** changelog init;

**2021-02-01 12:50** modified [setup.py](setup.py) to include `\graphicspath{{../fig/}}` 
as main graphics path

**2021-02-11** modified [setup.py](setup.py) to use template.tex file for latex 
template creation instead of inline comments. Uploaded latex template.tex file, 
serve as master file. Merged `working-on` branch on `master`. Removed old code 
from `esperienza_3_Moto_Proiettile`; added Changelog to README file. Uploaded .tex 
and .pdf file for e4 relazione. Uploaded .ipynb file for testing purposes, removed 
in gitignore.

**2021-02-13** made all code easier to read; all standard strings made accessible
in header as constant, so they cannot be modified. Added dict to [setup.py](setup.py)
to hold all log messaging strings.

**2021-02-15** made possible for new directories to auto-number themselves. Spaces 
in `title_underscore` now become `_` automatically. 

**2021-02-16** Merged branch `working-on` in branch `master`. Added ipynb ignore
file to .gitignore; Made all Important variable constants, and all those constant 
are initialized in the file header. Added ipynb ignore file to .gitignore;
Updated the input method, now supporting error handling notification if something 
goes wrong. Removed test.ipynb testing environment. Updated Changelog with every 
commit.

**2021-02-25** Merged branches, tag v4.3 latest release, removed numpy unnecessary 
import, added exit method for program.

**2021-03-06** Init v5.0/0 -> pre-relase: added tex file and README.

**2021-03-16** Final commit on e5 -> v5.2/0: published paper

**2021-03-31** Final commit on e6 -> v6.1.1: published paper
