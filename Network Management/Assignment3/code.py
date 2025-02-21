#! /usr/bin/python3

import sys
from easysnmp import Session

community_string_arg = sys.argv[1]
hostname_arg = sys.argv[2]
requested_items_arg = int(sys.argv[3])

snmp_session = Session(
    hostname=hostname_arg,
    community=community_string_arg,
    version=2,
    use_numeric=True,
    use_sprint_value=True
)

print("PID   P. Name           Mem. Allocated   No. Invoked")
print("---   -------------     --------------   -----------")

response = snmp_session.get_next('.1.3.6.1.4.1.9.9.109.1.4.1.1.1.7.20755')

names_list = []
pid_list = []
allocation_list = []
invocation_list = []

new_oid = str(response.oid) + '.' + str(response.oid_index)
split_response = new_oid.split('.')

while split_response[14] == '2':
    response = snmp_session.get(new_oid)
    service_name = response.value.strip('"')[:16]
    names_list.append(service_name)
    pid_list.append(response.oid_index)
    response = snmp_session.get_next(new_oid)
    new_oid = str(response.oid) + '.' + str(response.oid_index)
    split_response = new_oid.split('.')

allocation_oid = '.1.3.6.1.4.1.9.9.109.1.4.1.1.6.7.20755'
response = snmp_session.get_next(allocation_oid)
new_oid = str(response.oid) + '.' + str(response.oid_index)
split_response = new_oid.split('.')

while split_response[14] == '6':
    response = snmp_session.get(new_oid)
    allocation_list.append(response.value)
    response = snmp_session.get_next(new_oid)
    new_oid = str(response.oid) + '.' + str(response.oid_index)
    split_response = new_oid.split('.')

invocation_oid = '.1.3.6.1.4.1.9.9.109.1.4.1.1.8.7.20755'
response = snmp_session.get_next(invocation_oid)
new_oid = str(response.oid) + '.' + str(response.oid_index)
split_response = new_oid.split('.')

while split_response[14] == '8':
    response = snmp_session.get(new_oid)
    invocation_list.append(response.value)
    response = snmp_session.get_next(new_oid)
    new_oid = str(response.oid) + '.' + str(response.oid_index)
    split_response = new_oid.split('.')

size = len(pid_list)

if requested_items_arg > size:
    requested_items_arg = size

index = 0

while index < requested_items_arg:
    print('{:6}{:18}{:17}{:11}'.format(pid_list[index], names_list[index], allocation_list[index], invocation_list[index]))
    index += 1
