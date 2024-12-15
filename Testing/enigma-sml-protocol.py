filename = 'stromzaehlerFZS-01.log'

def parse_line(l):
    # 'l' will be of class string
    # it must be typecast to int to make calculations and bitwise operations 
    print(l)
    print()


with open(filename) as file:
    # 'line' will be of class str
    for line in file:
        #parse_line(line)
        pass

parse_line(line)
print(type(line))
print(f'Hello World!')