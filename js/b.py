import os
import logging


files = os.listdir(os.getcwd())
files = [i for i in files if i.endswith(".md")]

for i in files:
    with open(i, 'r+') as f:
        lines = f.readlines()
        split_num = []
        for k, j in enumerate(lines):
            if "━━" in j:
                split_num.append(k)

        print(split_num)
        if len(split_num) >= 2:
            f.seek(0)
            f.truncate()
            title = lines[split_num[0]-3].strip()
            print("title: ", title)
            f.writelines(["# ", title, "\n"])
            f.writelines(lines[split_num[0]+1: split_num[1]])

