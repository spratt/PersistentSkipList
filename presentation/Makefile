all:	slides.pdf clean_extras open

template:	conference-ornate-20min.en.pdf
	open conference-ornate-20min.en.pdf || okular conference-ornate-20min.en.pdf

conference-ornate-20min.en.pdf:	conference-ornate-20min.en.tex
	pdflatex conference-ornate-20min.en.tex
	pdflatex conference-ornate-20min.en.tex
	okular conference-ornate-20min.en.pdf

open:	slides.pdf
	open slides.pdf || okular slides.pdf

slides.pdf:	slides.tex
	pdflatex -halt-on-error slides.tex
	pdflatex slides.tex

notes:	notes.pdf
	open notes.pdf || okular notes.pdf

notes.pdf:	notes.tex
	pdflatex notes.tex

clean:	clean_extras clean_pdf

clean_extras:
	rm -f *.log *.aux *.out *.nav *.snm *.toc

clean_pdf:
	rm -f *.pdf
