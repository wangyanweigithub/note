class B(object):
    def __init__(self, name):
        self.name = name
        self.process = []


class A(object):
    def __init__(self, name):
        self.name = name

    def test(self):
        print(self.name)

    def run(self):
        b = B('test_b')
        b.process.append(self.test)
        return b

def main():
    a = A("A")
    b = a.run()
    b.process[0]()
    print(b.process[0])


main()
