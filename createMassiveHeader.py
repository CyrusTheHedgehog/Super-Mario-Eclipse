import re


f1 = open("funcs.hxx", "w")

pattern = re.compile(r"\b[0-9A-F]+\b \b[0-9A-F]+\b \b[0-9A-F]+\b", re.I | re.S | re.M)
with open('marioEU.MAP', 'r') as f:
    for line in f:
        x = re.search(pattern, line)
        if x:
            try:
                if line.split()[4][0] != "." and line.split()[4][0] != "@" and line.split()[4][0] != "(": 
                    output = "#define "+line.split()[4]+" ((int(*)(...))0x"+line.split()[2]+")\n"
                    f1.write(output)
            except:
                pass
f1.close()