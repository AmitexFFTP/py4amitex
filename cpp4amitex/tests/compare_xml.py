from sys import argv, stdout
from re import match
from xml.etree.ElementTree import canonicalize, fromstring, tostring
from difflib import unified_diff

if len(argv) >= 3:
    genedFile = argv[1]
    refFile = argv[2]
    def comparableXML(path):
        xmlstr = canonicalize(from_file=path, strip_text=True)
        tree = fromstring(xmlstr)
        # Tree order does not matter for AMITEX
        def sortChilds(elem):
            for child in elem:
                sortChilds(child)
            elem[:] = sorted(elem, key=lambda child: child.tag)

        def regularizeAttrs(elem):
            if elem.text:
                try:
                    flist = [str(float(x)) for x in elem.text.split()]
                    elem.text = " ".join(flist)
                except ValueError:
                    pass
            attrs = {}
            for key, value in elem.attrib.items():
                try:
                    # Regularise numbers
                    attrs[key] = str(float(value))
                except ValueError:
                    # AMITEX attributes values are case-insensitive (e.g. Value="True" <=> Value="true")
                    attrs[key] = value.lower()
            elem.attrib.update(attrs)
            for child in elem:
                regularizeAttrs(child)
        sortChilds(tree)
        # indent(tree)
        regularizeAttrs(tree)
        return tostring(tree, encoding="unicode")
    
    gened = comparableXML(argv[1])
    ref = comparableXML(argv[2])
    if gened == ref:
        exit(0)
    else:
        for line in unified_diff(gened.splitlines(), ref.splitlines(), genedFile, refFile, n=1):
            if line and line[-1] == '\n':
                stdout.write(line)
            else:
                print(line)
        exit(1)
else:
    exit(2)