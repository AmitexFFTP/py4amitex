from pathlib import Path
from xml.etree.ElementTree import canonicalize, fromstring, tostring
from difflib import unified_diff

testsdir = Path(__file__).parent.parent.parent.parent / "tests"


def _toFloat(s: str) -> float:
    ret = 0.0
    try:
        ret = float(s)
    except ValueError:
        # numbers like 0.49801817-310 are invalid, why Fortran outputs them ?
        pass
    return ret


def extractStressStrain(prefix):
    stress = []
    strain = []

    with open(prefix + ".std", encoding="utf-8") as file:
        for line in file:
            if line and line[0] == "#":
                continue
            vals = line.split()
            stress.append([_toFloat(tmp) for tmp in vals[1:7]])
            strain.append([_toFloat(tmp) for tmp in vals[7 : 7 + 6]])
    return stress, strain


def extractFluxGrad(prefix):
    flux = []
    grad = []

    with open(prefix + ".std", encoding="utf-8") as file:
        for line in file:
            if line and line[0] == "#":
                continue
            vals = line.split()
            flux.append([_toFloat(tmp) for tmp in vals[1:4]])
            grad.append([_toFloat(tmp) for tmp in vals[4 : 4 + 3]])
    return flux, grad


def compareXML(genedXML, refXML):
    def toComparableXML(xmlStr):
        xmlstr = canonicalize(xmlStr, strip_text=True)
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

    gened = toComparableXML(genedXML)
    ref = toComparableXML(refXML)
    ok = gened == ref
    text = ""
    if not ok:
        for line in unified_diff(
            gened.splitlines(), ref.splitlines(), "generated.xml", "reference.xml", n=1
        ):
            text += line
            if line[-1] != "\n":
                text += "\n"
        print(text)
    return ok


def compareXMLFiles(path, refpath):
    with open(path, "r", encoding="utf-8") as file:
        xmlStr = file.read()
    with open(refpath, "r", encoding="utf-8") as file:
        refStr = file.read()
    return compareXML(xmlStr, refStr)
