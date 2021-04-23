import re


f1 = open("./linker/eu.map", "w")

pattern = re.compile(r"\b[0-9A-F]+\b \b[0-9A-F]+\b \b[0-9A-F]+\b", re.I | re.S | re.M)
with open('./bin/debug/eu/marioEU.MAP', 'r') as f:
    for line in f:
        x = re.search(pattern, line)
        if x:
            try:
                if line.split()[4][0] != "." and line.split()[4][0] != "@" and line.split()[4][0] != "(": 
                    output = line.split()[4]+"=0x"+line.split()[2]+"\n"
                    f1.write(output)
            except:
                pass
f1.close()