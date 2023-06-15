import re
import os
import json
import subprocess
import argparse

def main():
    parser = argparse.ArgumentParser(description = "A compiler for the Cvenska programing language")

    parser.add_argument('filename',
                        help = "The Cvenska file that should be compiled.")

    parser.add_argument('-v', '--verbose',
                        action='store_true',
                        help = "Prints information as the program runs.")

    parser.add_argument('-sc', '--save_c',
                        action='store_true',
                        help = "Keeps the temporary C file for inspection.")

    parser.add_argument('-o', '--output',
                        metavar = "filename", default = "a.out",
                        help = "Output file for program")

    args = parser.parse_args()

    if args.verbose:
        print(f"Reading from file {args.filename}...")
    f = open(args.filename, "r")
    program = f.read()
    program = " " + program
    f.close()
    if args.verbose:
        print(f"Finished reading from file {args.filename}")

    lexicon_file = open("lexicon.json", "r")
    lexicon = json.load(lexicon_file)

    if args.verbose:
        print(f"Begin regex... ")
    for module in lexicon.keys():        
        if args.verbose:
            print(f"Start {module}")
        for c, cv in lexicon[module].items():
            if (c == cv):
                continue
            special_sym = r"([\s~@#\$%\^\&\*\(\)\-\+=\{\}\[\];:\'\"/\?\.>,<\\\|])"
            pat = special_sym+c+special_sym
            if re.search(pat, program):
                i = 0
                while True:
                    if not re.search(special_sym+c+str(i)+special_sym, program):
                        program = re.sub(pat, r"\1"+c+str(i)+r"\2", program)
                        break
                    i += 1 
            pat = special_sym+cv+special_sym
            program = re.sub(pat, r"\1"+c+r"\2", program)
    
    program = program[1:]
    if args.verbose:
        print(f"Finished regex")

    if args.verbose:
        print(f"Create temporary C file")
    with open("temp.c", "w+") as temp_c:
        temp_c.write(program)

    if args.verbose:
        print(f"Run gcc with flags: ")
    gcc = subprocess.run(["gcc", "temp.c", "-o", args.output])

    if not args.save_c:
        if args.verbose:
            print(f"Remove temporary C file")
        os.remove("temp.c") 

if __name__ == "__main__":
    main()
