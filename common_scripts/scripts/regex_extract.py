#!/usr/bin/python

r"""
e.g. extract bazel targets from text:

    regex_extract.py '(//[\w/:]+)' /tmp/output.txt | sort
"""

import re

from common_scripts.text_processor import TextProcessor


class Custom(TextProcessor):
    def __init__(self):
        TextProcessor.__init__(self, skipArgs = True, description = "Extract stuff")
        self.parser.add_argument('pattern', type = str)
        self.addArguments()

    def process(self, oldText):
        ms = re.findall(self.args.pattern, oldText, flags = re.M)
        lines = []
        for m in ms:
            if isinstance(m, str):
                lines.append(m)
            else:
                lines.append(m.groups())
        return "\n".join(lines) + "\n"


if __name__ == "__main__":
    Custom().main()
