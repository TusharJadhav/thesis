del Tushar_Thesis.aux
del Tushar_Thesis.bbl
del Tushar_Thesis.dvi
del Tushar_Thesis.lof
del Tushar_Thesis.lot
del Tushar_Thesis.blg
del Tushar_Thesis.ps
del Tushar_Thesis.log
del Tushar_Thesis.toc
pdflatex Tushar_Thesis.tex
bibtex Tushar_Thesis.aux
pdflatex Tushar_Thesis.tex
XCOPY ".\Precompiled\Tushar_Thesis.lof" . /Y
XCOPY ".\Precompiled\Tushar_Thesis.lot" . /Y
XCOPY ".\Precompiled\Tushar_Thesis.toc" . /Y
pdflatex Tushar_Thesis.tex
del Tushar_Thesis.aux
del Tushar_Thesis.bbl
del Tushar_Thesis.dvi
del Tushar_Thesis.lof
del Tushar_Thesis.lot
del Tushar_Thesis.blg
del Tushar_Thesis.ps
del Tushar_Thesis.log
del Tushar_Thesis.toc
cls
Tushar_Thesis.pdf


