import sys

def read_hex_file(file_path):
    try:
        with open(file_path, 'r') as file:
            hex_values = file.read().split()
            return hex_values
    except FileNotFoundError:
        print(f"File not found: {file_path}")
    except Exception as e:
        print(f"An error occurred: {e}")
    return None

def calculate_size_and_start(hex_values):
    hex_count = len(hex_values)
    if int(hex_values[1], 16) == hex_count - 2:
        return hex_count, 2
    elif int(''.join(hex_values[2:4]), 16) == hex_count - 4:
        return hex_count, 4
    elif int(''.join(hex_values[2:5]), 16) == hex_count - 6:
        return hex_count, 5
    return None, None

def determine_pdu_type(hex_values, start):
    pdu_mapping = {
        'a0': 'Get Request',
        'a1': 'Get_Next Request',
        'a2': 'Get Response',
        'a3': 'Set Request',
        'a4': 'Obsolete',
        'a5': 'Get Bulk Request',
        'a6': 'Inform Request',
        'a7': 'Trap'
    }
    pdu_type_hex = ''.join(hex_values[start:start + 1])
    return pdu_mapping.get(pdu_type_hex, 'Unknown PDU Type')

def parse_request_id(hex_values, string_end):
    hex_integer = int(''.join(hex_values[string_end:string_end + 2]), 16)
    bits = format(hex_integer, 'b').split()
    
    pdulength = 4 if bits[0][8] == '1' else 2
    request_id_start = string_end + pdulength + 1
    request_id_length = int(hex_values[request_id_start], 16)
    request_id = ''.join(hex_values[request_id_start + 1:request_id_start + request_id_length + 1])
    
    return int(request_id, 16), request_id_start + request_id_length + 1

def main():
    if len(sys.argv) < 2:
        print("Usage: python script.py <filename>")
        sys.exit(1)

    file_path = sys.argv[1]
    hex_values = read_hex_file(file_path)

    if hex_values is None:
        sys.exit(1)

    size, start = calculate_size_and_start(hex_values)

    if size is None or start is None:
        print("Invalid hex values format.")
        sys.exit(1)

    version = int(hex_values[start + 1], 16) + 1

    print(f"Field            Value\n---------------  -----------\nSize of Message  {size} Bytes")
    print(f"Version          {version}")

    community_string_length = int(hex_values[start + 4], 16)
    string_end = start + 4 + community_string_length + 1
    community_string = bytes.fromhex(''.join(hex_values[start + 5:string_end])).decode("ASCII")
    print(f"Community        {community_string}")

    pdu_type = determine_pdu_type(hex_values, string_end)
    print(f"PDU Type         {pdu_type}")

    request_id, final = parse_request_id(hex_values, string_end)
    print(f"Request ID       {request_id}")
    if pdu_type == 'Get Bulk Request':
        print(f"Non-Repeaters    {int(hex_values[final + 2], 16)}")
        print(f"Max Repetitions  {int(hex_values[final + 5], 16)}")
    else:
        print(f"Error Status     {int(hex_values[final + 2], 16)}")
        print(f"Error Index      {int(hex_values[final + 5], 16)}")

if __name__ == "__main__":
    main()

