
### Documentation py4amitex

This documentation needs `sphinx-build` and `texlive`.

To have `html` and `pdf` documentations in installed packages,
`Makefile` is appended:
 - make html -> to copy build/html in py4amitex/py4amitex/doc.
 - make latexpdf -> to copy build/html in py4amitex/py4amitex/doc.


#### Documentation html

```
cd py4amitec/doc
make html

# Display documentation html
firefox build/html/index.html &
```


#### Documentation pdf

Needs package `texlive` up to date to get command `latexmk`,
see https://www.tug.org/texlive/quickinstall.html.

You may install package locally, without root privileges.

```
# for example:
export INFOPATH=/data/tmplgls/wambeke/share/texlive/2017/texmf-dist/doc/info
export MANPATH=/data/tmplgls/wambeke/share/texlive/2017/texmf-dist/doc/man
export PATH=/data/tmplgls/wambeke/share/texlive/2017/bin/x86_64-linux:${PATH}

cd py4amitex/doc
make latexpdf

# Display documentation pdf
evince build/latex/py4amitex.pdf &
```
