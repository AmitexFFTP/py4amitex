import argparse
import re
from textwrap import indent

def forReadLine(file):
    line = file.readline()
    if not line:
        return line
    if line.lstrip():
        line = line.lstrip()
    if (not re.match(r"^!", line)) and line.rstrip().endswith('&'):
        result = line.rstrip()[:-2]
        line = file.readline().lstrip()
        while line.rstrip().endswith('&'):
            result += line.rstrip()[:-2]
            line = file.readline().lstrip()
        result += line
    else:
        result = line
    return result

class Module:
    def __init__(self, name, doc=""):
        self.name = name
        self.doc = doc

class Function:
    def __init__(self, name, args, retvar, types, doc=""):
        self.name = name
        self.args = args
        self.rettype =  types[retvar] if retvar else "void"
        self.types = types
        self.doc = doc

    def argsDecl(self):
        for arg in self.args:
            if arg:
                typ = self.types[arg]
                yield f"{typ} {arg}"

def getType(main, spec, attrs):
    ints = {"c_int": "int", "c_size_t": "size_t", "int32": "int32_t", "int64": "int64_t"}
    reals = {"c_float": "float", "c_double": "double", "real32": "float", "real64": "double"}
    
    if main == "integer":
        if spec:
            result = ints.get(spec, f"fortran_{spec}")
        else:
            result = "int"
    elif main == "real":
        if spec:
            result = reals.get(spec, f"fortran_{spec}")
        else:
            result = "float"
    elif main == "character":
        result = "char"
    elif main == "logical":
        if spec == "c_bool":
            result = "bool"
        else:
            result = ints.get(spec, "int")
    elif main == "type":
        result = spec
    else:
        result = "void"

    if re.search(r"(intent\((out|inout)\))", attrs) or (not "value" in attrs):
        result = f"{result}*"
    elif "intent(in)" in attrs:
        result = f"const {result}*"
    return result

def parseFortranFile(file):
    line = forReadLine(file)
    modules = []
    functions = []
    while line:
        comment = ""
        m = re.match(r"^!>\s*(.*)$", line)
        if m:
            comment = m.group(1) + '\n'
            line = forReadLine(file)
            m = re.match(r"^!!\s*(.*)$", line)
            while m:
                comment += m.group(1) + '\n'
                line = forReadLine(file)
                m = re.match(r"^!!\s*(.*)$", line)
            if not re.match(r"^[a-zA-Z]", line):
                comment = ""
        if m := re.match(r"^module\s*(\w+)", line):
            modules.append(Module(m.group(1), doc=comment))
        elif m := re.match(r"^(function|subroutine)\s*(\w+)\(\s*((?:\w+)?\s*(?:,\s*(?:\w+)\s*)*)\)\s*bind\(C\)\s*(result\(\w+\))?", line):
            args = [s.strip() for s in m.group(3).split(',')]
            if m.group(4):
                retvar = re.match(r"result\((\w+)\)", m.group(4)).group(1)
            else:
                retvar = None
            types = {}
            # Should recurse for 'contains' stuff
            line = forReadLine(file)
            while not re.match(f"^end {m.group(1)}", line):
                if var := re.match(r"^(integer|real|logical|type|character)(\([_a-z0-9\*:]+\))?(.*)::\s*(\w+(?:\(.*\))?)\s*$", line):
                    for varDef in var.group(4).split(','):
                        if varDefMatch := re.match(r"(\w+)(\(.*\))?", varDef):
                            name = varDefMatch.group(1)
                        if name in args or name == retvar:
                            if name == retvar:
                                attrs = var.group(3) +  ",value" # return by value
                            else:
                                attrs = var.group(3)
                            typeSpec = var.group(2)[1:-1] if var.group(2) else None
                            types[name] = getType(var.group(1), typeSpec, attrs)
                line = forReadLine(file)
            functions.append(Function(m.group(2), args, retvar, types, doc=comment))

        line = forReadLine(file)
    return (modules, functions)

def genHeader(modules, functions, filename):
    result = """#ifndef __AMITEX_API_HEADER__
#define __AMITEX_API_HEADER__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

"""
    result += f"//! \\file {filename}\n"
    for m in modules:
        result += indent(m.doc, "//! ", lambda line: True)
    result += "\n"
    for fn in functions:
        result += indent(fn.doc, "//! ", lambda line: True)
        result += f"{fn.rettype} {fn.name}({', '.join(arg for arg in fn.argsDecl())});\n\n"
    result += """#ifdef __cplusplus
}
#endif

#endif // __AMITEX_API_HEADER__
"""
    return result

parser = argparse.ArgumentParser(description="Generate a C header file from the fortran bindings source")
parser.add_argument('input_file')
parser.add_argument('output_file')
args = parser.parse_args()

with open(args.input_file, "r", encoding="utf-8") as file:
    (modules, functions) = parseFortranFile(file)
    for m in modules:
        print(f"Found module {m.name}")

if modules or functions:
    with open(args.output_file, "w", encoding="utf-8") as file:
        file.write(genHeader(modules, functions, args.output_file))
