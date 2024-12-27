filename = 'stromzaehlerFZS-01.log'

def parse_line_to_array(l):
    # 'l' will be of class string
    # it must be typecast to int to make calculations and bitwise operations 
    list_hex_string = l.split()
    sml_package = []
    for i in list_hex_string:
        if(i=="HIT"):
            pass
        else:
            sml_package.append(int(i, 16))
    return sml_package

# CCITT CRC16
def compute_crc16(l):
    # 'l' will be of class list with integer elements 
    pass

def is_sml_message_str(l):
    # 'l' will be of class str
    # look out for sequence of '01 01 76'
    if( l[0:8].find('01 01 76') != -1):
        return True
    else:
        return False

def is_sml_message(l):
    # 'l' will be of class list with integer elements 
    # look out for sequence of '01 01 76'
    if( l[0] == 0x01 and l[1] == 0x01 and l[2] == 0x76 ):
        return True
    else:
        return False

def decode_sml_message_instantaneous_power(l):
    # 'l' will be of class string
    key_power = '07 01 00 10 07 00 FF'
    indexPower = l.find(key_power)
    power = 0
    if(indexPower != -1):
        subMessage = l[indexPower:indexPower+100]
        array = parse_line_to_array(subMessage)
        power = (array[14] << 8) | (array[15] << 0)
    return power

def decode_sml_message_energie_consumption(l):
    key_energie_consumed = '07 01 00 01 08 00 FF'
    indexEnergiePos = l.find(key_energie_consumed)
    energiePos = 0
    if(indexEnergiePos != -1):
        subMessage = l[indexEnergiePos:indexEnergiePos+100]
        array = parse_line_to_array(subMessage)
        energiePos = (array[24] << 24) | (array[25] << 16) | (array[26] << 8) | array[27]
    return energiePos

def decode_sml_message_energie_injection(l):
    key_energie_generated = '07 01 00 02 08 00 FF'
    indexEnergieNeg = l.find(key_energie_generated)
    energieNeg = 0
    if(indexEnergieNeg != -1):
        subMessage = l[indexEnergieNeg:indexEnergieNeg+100]
        array = parse_line_to_array(subMessage)
        energieNeg = (array[21] << 16) | (array[22] << 8) | (array[23] << 0)
    return energieNeg
    
if( __name__ == "__main__"):
    with open(filename) as file:
        # 'line' will be of class str
        for line in file:
            if( is_sml_message_str(line) ):
                energie1 = decode_sml_message_energie_consumption(line)
                energie2 = decode_sml_message_energie_injection(line)
                power = decode_sml_message_instantaneous_power(line)
                print(f'Energie Pos: {energie1/10000:.3f} kWh  Energie Neg: {energie2/10000:.3f} kWh power: {power} W')

    array = parse_line_to_array(line)
    print(f'end of script')