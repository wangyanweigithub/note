import os
import


class Manager(object):
    def __init__(self, name, fileends=None, *args):
        self.path = os.getcwd()
        self.fileends = [] if not fileends else fileends
        self.files = None
        self.name = name

    @protety
    def files(self, filters=None):
        if self.files:
            return self.files

        files = os.list(self.path)
        files = [i for i in files if i.split(".")[-1] in self.fileends]
        self.files = files
        return self.files

    def concat(self, out=None):
        result = []
        for i in self.files:
            with open(i, 'r') as f:
                for line in f:
                    if line.startswith("<!"):
                        continue
