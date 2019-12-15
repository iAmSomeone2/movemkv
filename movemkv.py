#!/usr/bin/env python3

from os import path, rename
from sys import argv, exit

_MONTH = {
    1: "01_January",
    2: "02_February",
    3: "03_March",
    4: "04_April",
    5: "05_May",
    6: "06_June",
    7: "07_July",
    8: "08_August",
    9: "09_September",
    10: "10_October",
    11: "11_November",
    12: "12_December"
}

def determineLocation(fileName):
    baseName = str(path.basename(fileName))
    baseName = baseName.split('.')[0]

    year = baseName[0:4]
    month = int(baseName[4:6])
    day = baseName[6:8]
    return year + path.sep + _MONTH[month] + path.sep + day + path.sep + str(path.basename(fileName))

if __name__ == "__main__":
    if len(argv) < 3:
        print("Usage: movemkv.py <filename> <outdir>")
        exit(1)

    fileLoc = determineLocation(argv[1])
    print(path.join(argv[2], fileLoc))