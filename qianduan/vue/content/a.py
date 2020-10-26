import os


abandon_str = "<!-- vim-markdown-toc -->\n"


def get_name():
    with open("index.md", "r") as index:
        toc = index.readlines()

    index = [i for i in toc if i.endswith(".md\n")]
    index = [i.split(" ")[-1].strip() for i in index]
    return index


def generator_vue_txt():
    files = get_name()
    with open("vue.txt", "w") as vue:
        for i in files:
            with open(i, "r") as f:
                lines = f.readlines()
            if abandon_str in lines:
                lines = lines[lines.index(abandon_str, 3)+1:]
            vue.writelines(lines)


generator_vue_txt()
