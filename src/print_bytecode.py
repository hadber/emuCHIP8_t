import sys

# make sure your first argument is the path to your *.ch8 program
# i am NOT going to check it
with open(sys.argv[1], "rb") as f:
    i = 0
    while (byte := f.read(2)):
        print(hex(i).upper(), "\t", byte.hex().upper())
        i += 1