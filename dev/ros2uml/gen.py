import os
from os.path import normpath, basename, split
import re
from colorama import init as colorama_init
from colorama import Fore, Style

import interface_parsing


# Script for generating plantUML text files from ros2 interface description

# ros2 directory for interface description
SRV = 'srv'
ACTION = 'action'
MSG = 'msg'

uml_file = 'ros2_interfaces.txt'

colorama_init()

dir_of_interest = (MSG, ACTION, SRV)
ext_of_interest = ['.' + ext for ext in dir_of_interest]

def parse(content, node, int_typem, file):
    plant_uml = ''

    plant_uml += f'interface {file} {{\n'

    for line in content.splitlines():
        col = line.split()
        plant_uml += '  ' + ' '.join(col) + '\n'

    plant_uml += f'}}\n\n' 
    return plant_uml

def get_list_of_files():
    interfaces = list()

    for dirpath, dirnames, filenames in os.walk('.'):
        base = os.path.basename(dirpath)
        second_up = os.path.basename(os.path.split(dirpath)[0])

        if(base in dir_of_interest):
            for file in filenames:
                root, ext = os.path.splitext(file)

                if(ext in ext_of_interest):
                    second_up = re.sub(r'_interface$', '', second_up)

                    full_path = os.path.join(dirpath, file)
                    interfaces.append([second_up, base, root, full_path])

    return interfaces


interfaces = get_list_of_files();

with open(uml_file, 'w') as output:

    print('@startuml', file=output)
    print('skin rose', file=output)
    print('title Ros Interfaces Diagram', file=output)
    print('', file=output)

    past_node = ''
    for node, int_type, file, full_path in interfaces:
        if int_type != MSG:
            continue

        print(f'{Fore.GREEN}working on: {Style.RESET_ALL}', node, int_type, file, full_path)

        if past_node != node:
            if past_node != '':
                print(f'}}\n\n', file=output)

            print(f'node {node} {{', file=output)


        content_uml = ''
        with open(full_path) as f:
            content = f.read()
            content_uml = parse(content, node, int_type, file)

        output.write(content_uml)

        past_node = node


    print('@enduml', file=output)

