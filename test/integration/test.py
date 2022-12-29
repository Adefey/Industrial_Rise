import unittest
from subprocess import Popen
from functools import reduce

def run_program():
    p = Popen(["./build/industrial_map_reduce"])
    p.wait()


class IntegrationalTests(unittest.TestCase):
    def test_texts(self):
        run_program()
        with open("./example_data/text.txt", "r", encoding="UTF-8") as f:
            lines = f.readlines()
        lines = map(lambda x: x.split(), lines)
        text = reduce(lambda a, b: a + b, lines)
        
        for reducer in range(10):
            with open(f"./result_dir/reduced_{reducer}", "r", encoding="UTF-8") as f:
                lines = f.readlines()
            for line in lines:
                word, count = line.split(": ")
                if word == "":
                    continue
                self.assertIn(word, text)
                        

if __name__ == '__main__':
    unittest.main()
