#!/usr/local/bin/python3

import sys
from time import sleep
from netmiko import ConnectHandler

def main():
    if len(sys.argv)<3:
                print("----ADD an IP address and lines----")
                return
    device = {
        'device_type': 'cisco_ios',
        'host': sys.argv[1],
        'username': 'nes470user',
        'password': 'nes470passwd',
    }
    connection = ConnectHandler(**device)

    output = connection.send_command("show processes")
    output2 = connection.send_command("show processes memory\n")
    process = output.splitlines()[2:]
    process2 = output2.splitlines()[4:]
    secondArray = []
    i = 0
    for x in process:
        y = x.split()
        y2 = process2[i].split()
        firstArray = []
        firstArray.append(y[0])
        firstArray.append(y[7:])
        firstArray.append(y2[2])
        firstArray.append(y[4])
        secondArray.append(firstArray)
        i += 1

    print("{:5}{:20}{:16}{:13}".format("PID", "P. Name", "Mem. Allocated", "No. Invoked"))
    print("-" * 3 + "  " + "-" * 18 + "  " + "-" * 14 + "  " + "-" * 11)

    newArray = []
    for i in range(0, int(sys.argv[2])):
        newArray.append(secondArray[i])

    for z in newArray:
        print("{:5}{:20}{:16}{:13}".format(z[0], ' '.join(z[1]), z[2], z[3]))

    connection.disconnect()

if __name__ == "__main__":
    main()
