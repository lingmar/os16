TEX = xelatex -shell-escape -interaction=nonstopmode -file-line-error

.PHONY: all clean

all: answers.pdf

answers.pdf: answers.tex
	latexmk -pdf -pdflatex="$(TEX)" answers.tex
	$(TEX) answers.tex

clean:
	latexmk -CA
