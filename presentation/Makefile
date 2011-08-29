all:	slides.pdf clean_extras open

open:	slides.pdf
	open slides.pdf || okular slides.pdf || xpdf slides.pdf

slides.pdf:	slides.tex
	pdflatex slides.tex

clean:	clean_extras clean_pdf

clean_extras:
	rm -f *.log *.aux *.out *.nav *.snm *.toc

clean_pdf:
	rm -f *.pdf
