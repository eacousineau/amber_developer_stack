#!/usr/bin/python

import re
from common_scripts.text_processor import TextProcessor

class Custom(TextProcessor):
    def __init__(self):
        TextProcessor.__init__(self, skipArgs = True, description = "Swap ")
        self.parser.add_argument('a', type = str)
        self.parser.add_argument('b', type = str)
        self.parser.add_argument('temp', type = str)
        self.addArguments()
        
    def process(self, oldText):
        return oldText.replace(self.args.a, self.args.temp).replace(self.args.b, self.args.a).replace(self.args.temp, self.args.b)

Custom().main()
