README file
===========

Folder structure
----------------
main folder `/esperienza_4_Pendolo`

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

Lettura di file dati per i grafici:
-----------------------------------

I file ed_dati.txt, rp_dati.txt, ms_dati.txt sono nel formato:

```
L_0 eL_0 T_0 eT_0 T2_0 eT2_0
...

...
L_N eL_N T_N eT_N T2_N eT2_N
```
dove le lunghezze **L+/-eL** sono in metri, mentre le oscillazioni sono in secondi e  secondi^2, per le oscillazioni al quadrato. 
