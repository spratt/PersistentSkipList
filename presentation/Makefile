all:	slides.pdf clean_extras open

template:
	pdflatex conference-ornate-20min.en.tex && okular conference-ornate-20min.en.pdf
	pdflatex conference-ornate-20min.en.tex && okular conference-ornate-20min.en.pdf

open:	slides.pdf
	open slides.pdf || okular slides.pdf || xpdf slides.pdf

slides.pdf:	slides.tex
	pdflatex slides.tex
	pdflatex slides.tex

clean:	clean_extras clean_pdf

clean_extras:
	rm -f *.log *.aux *.out *.nav *.snm *.toc

clean_pdf:
	rm -f *.pdf
