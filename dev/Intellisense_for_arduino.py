#/usr/bin/env python3

"""convert txt files with path string to include to vs code format"""

import os
print(os.getcwd())

PREF = r'/home/alexey/.arduino15/packages/rp2040/hardware/rp2040/3.6.0/'

with open('/home/alexey/.arduino15/packages/rp2040/hardware/rp2040/3.6.0/lib/platform_inc.txt', encoding="utf-8") as my_file:
    for line in my_file:
        line = line.strip().removeprefix('-iwithprefixbefore/')

        print(f'\t\t\t\t"{PREF}{line}",')
