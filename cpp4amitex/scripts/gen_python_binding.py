#!/usr/bin/env python3

"""
Generate a python binding for a class

:author: G. Bareigts
:copyright: CEA 2026
"""

import re
import textwrap
from argparse import ArgumentParser

import litgen

parser = ArgumentParser()
parser.add_argument("header", type=str, help="Path to an header file")

args = parser.parse_args()


# This code was extracted from [pybind11_mkdocs](https://github.com/pybind/pybind11_mkdoc),
# as Doxygen docstrings need to be transformed
def process_comment(comment: str) -> str:
    docstring_width = int(70)

    result = ""

    # Remove C++ comment syntax
    leading_spaces = float("inf")
    for s in comment.expandtabs(tabsize=4).splitlines():
        s = s.strip()
        if s.endswith("*/"):
            s = s[:-2].rstrip("*")
        if s.startswith("/*"):
            s = s[2:].lstrip("*!<")
        elif s.startswith("//"):
            s = s[2:].lstrip("/!<")
        elif s.startswith("*"):
            s = s[1:]
        if len(s) > 0:
            leading_spaces = min(leading_spaces, len(s) - len(s.lstrip()))
        result += s + "\n"

    if leading_spaces != float("inf"):
        result2 = ""
        for s in result.splitlines():
            result2 += s[int(leading_spaces):] + "\n"
        result = result2

    # Doxygen tags
    cpp_group = r"([^\s]+)"
    param_group = r"([\[\w:,\]]+)"

    s = result
    s = re.sub(r"[\\@][cp]\s+%s" % cpp_group, r"``\1``", s)
    s = re.sub(r"[\\@]a\s+%s" % cpp_group, r"*\1*", s)
    s = re.sub(r"[\\@]e\s+%s" % cpp_group, r"*\1*", s)
    s = re.sub(r"[\\@]em\s+%s" % cpp_group, r"*\1*", s)
    s = re.sub(r"[\\@]b\s+%s" % cpp_group, r"**\1**", s)
    s = re.sub(r"[\\@]ingroup\s+%s" % cpp_group, r"", s)
    s = re.sub(
        r"[\\@]param%s?\s+%s" % (param_group, cpp_group),
        r"\n\n$Parameter ``\2``:\n\n",
        s,
    )
    s = re.sub(
        r"[\\@]tparam%s?\s+%s" % (param_group, cpp_group),
        r"\n\n$Template parameter ``\2``:\n\n",
        s,
    )

    # Remove class and struct tags
    s = re.sub(r"[\\@](class|struct)\s+.*", "", s)

    for in_, out_ in {
        "returns": "Returns",
        "return": "Returns",
        "authors": "Authors",
        "author": "Author",
        "copyright": "Copyright",
        "date": "Date",
        "remark": "Remark",
        "sa": "See also",
        "see": "See also",
        "extends": "Extends",
        "throws": "Throws",
        "throw": "Throws",
    }.items():
        s = re.sub(r"[\\@]%s\s*" % in_, r"\n\n$%s:\n\n" % out_, s)

    s = re.sub(r"[\\@]details\s*", r"\n\n", s)
    s = re.sub(r"[\\@]brief\s*", r"", s)
    s = re.sub(r"[\\@]short\s*", r"", s)
    s = re.sub(r"[\\@]ref\s*", r"", s)

    s = re.sub(
        r"[\\@]code\s?(.*?)\s?[\\@]endcode", r"```\n\1\n```\n", s, flags=re.DOTALL
    )
    s = re.sub(
        r"[\\@]warning\s?(.*?)\s?\n\n", r"$.. warning::\n\n\1\n\n", s, flags=re.DOTALL
    )
    # Deprecated expects a version number for reST and not for Doxygen. Here the first word of the
    # doxygen directives is assumed to correspond to the version number
    s = re.sub(
        r"[\\@]deprecated\s(.*?)\s?(.*?)\s?\n\n",
        r"$.. deprecated:: \1\n\n\2\n\n",
        s,
        flags=re.DOTALL,
    )
    s = re.sub(
        r"[\\@]since\s?(.*?)\s?\n\n", r".. versionadded:: \1\n\n", s, flags=re.DOTALL
    )
    s = re.sub(r"[\\@]todo\s?(.*?)\s?\n\n", r"$.. todo::\n\n\1\n\n", s, flags=re.DOTALL)

    # HTML/TeX tags
    s = re.sub(r"<tt>(.*?)</tt>", r"``\1``", s, flags=re.DOTALL)
    s = re.sub(r"<pre>(.*?)</pre>", r"```\n\1\n```\n", s, flags=re.DOTALL)
    s = re.sub(r"<em>(.*?)</em>", r"*\1*", s, flags=re.DOTALL)
    s = re.sub(r"<b>(.*?)</b>", r"**\1**", s, flags=re.DOTALL)
    s = re.sub(r"[\\@]f\$(.*?)[\\@]f\$", r":math:`\1`", s, flags=re.DOTALL)
    s = re.sub(r"<li>", r"\n\n* ", s)
    s = re.sub(r"</?ul>", r"", s)
    s = re.sub(r"</li>", r"\n\n", s)

    s = s.replace("``true``", "``True``")
    s = s.replace("``false``", "``False``")

    # Re-flow text
    wrapper = textwrap.TextWrapper()
    wrapper.expand_tabs = True
    wrapper.replace_whitespace = True
    wrapper.drop_whitespace = True
    wrapper.width = docstring_width
    wrapper.initial_indent = wrapper.subsequent_indent = ""

    result = ""
    in_code_segment = False
    for x in re.split(r"(```)", s):
        if x == "```":
            if not in_code_segment:
                result += "```\n"
            else:
                result += "\n```\n\n"
            in_code_segment = not in_code_segment
        elif in_code_segment:
            result += x.strip()
        else:
            for y in re.split(r"(?: *\n *){2,}", x):
                wrapped = wrapper.fill(re.sub(r"\s+", " ", y).strip())
                if len(wrapped) > 0 and wrapped[0] == "$":
                    result += wrapped[1:] + "\n"
                    wrapper.initial_indent = wrapper.subsequent_indent = " " * 4
                else:
                    if len(wrapped) > 0:
                        result += wrapped + "\n\n"
                    wrapper.initial_indent = wrapper.subsequent_indent = ""
    return result.rstrip().lstrip("\n")


def preprocess_code(code: str) -> str:
    def trans_comment(comment: str):
        return textwrap.indent(
            process_comment(comment) + "\n", "// ", predicate=lambda _: True
        )

    def is_line_comment(s: str) -> bool:
        return (
            s.startswith("///")
            or s.startswith("//!")
            or s.startswith("//!<")
            or s.startswith("//<")
        )

    result = ""
    incomment = False
    inblockcomment = False
    comment = ""
    for line in code.splitlines(keepends=True):
        s = line.strip()
        if incomment:
            if not is_line_comment(s):
                result += trans_comment(comment)
                result += line
                incomment = False
                comment = ""
            else:
                comment += line
        elif inblockcomment:
            if s.endswith("*/"):
                inblockcomment = False
                comment = ""
                result += trans_comment(comment)
            else:
                comment += line
        elif is_line_comment(s):
            incomment = True
            comment += line
        elif s.startswith("/**"):
            inblockcomment = True
            comment += line
        else:
            result += line
        pass
    return result


# with open(args.header, "r") as file:
#     print(preprocess_code(file.read()))

options = litgen.LitgenOptions()
options.srcmlcpp_options.code_preprocess_function = preprocess_code
options.namespaces_root = ["amitex"]
# template toXMLElement not needed to be exposed
options.fn_exclude_by_name__regex = "^toXMLElement$"
# To pass include guards
options.srcmlcpp_options.header_filter_acceptable__regex = "_H(_)?$|_HEADER(_)?$"
# We are still camelCase in Python
options.python_convert_to_snake_case = False

generated_code = litgen.generate_code_for_file(options, args.header)

print(generated_code.pydef_code)
print()
print(generated_code.glue_code)
