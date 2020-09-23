#!/usr/bin/env python
# -*- coding:utf-8 -*-

"""
from https://deusyss.developpez.com/tutoriels/Python/json_schema/#LIII-A
"""

import os
import json
from jsonschema import validate

_here = os.path.dirname(__file__)

def launch_validate(dict_to_test, dict_valid):
    try:
        validate(dict_to_test, dict_valid)
    except Exception as valid_err:
        print("Validation KO\n{}\n".format(valid_err))
        raise valid_err
    else:
        # Realise votre travail
        print("Validation OK\n")


if __name__ == '__main__':
    case = "essai_1"

    # load schema
    f_schema_json = os.path.join(_here, "%s_schema.json" % case)
    with open(f_schema_json, "r") as fichier:
        dict_valid = json.load(fichier)

    # load data ok
    f_json = os.path.join(_here, "%s_ok.json" % case)
    with open(f_json, "r") as f:
        dict_to_test_ok = json.load(f)

    # load data ko
    f_json = os.path.join(_here, "%s_ko.json" % case)
    with open(f_json, "r") as f:
        dict_to_test_ko = json.load(f)

    launch_validate(dict_to_test_ok, dict_valid)
    launch_validate(dict_to_test_ko, dict_valid)
