filename = 'stromzaehlerFZS-01.log'

def parse_line(l):
    # 'l' will be of class string
    # it must be typecast to int to make calculations and bitwise operations 
    list_hex_string = l.split()
    sml_package = []
    for i in list_hex_string:
        if(i=="HIT"):
            print(i)
        else:
            sml_package.append(int(i, 16))
    return sml_package

# CCITT CRC16
def compute_crc16(l):
    # 'l' will be of class list with integer elements 
    pass

def is_sml_message(l):
    # 'l' will be of class list with integer elements 
    if( l[0] == 0x01 and l[1] == 0x01 and l[2] == 0x76 ):
        return True
    else:
        return False

def decode_sml_message(l):
    # 'l' will be of class list with integer elements 
    pass

if( __name__ == "__main__"):
    with open(filename) as file:
        # 'line' will be of class str
        for line in file:
            #parse_line(line)
            pass

    array = parse_line(line)

    print(type(line))
    print(type(array))
    print(is_sml_message(array))
    print(f'end of script')