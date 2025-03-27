import string

msg = "Kf tvjt Npj"

caracs = string.ascii_letters

msf_f = ""

for c in range(26):
    msf_f = ""

    for x in msg:
        if x == " ":
            msf_f += " "

        elif x in caracs:
            index = caracs.index(x)

            new_index = (index + c) % len(caracs)
            lettre = caracs[new_index]

            """if lettre.upper():
                msf_f += lettre.lower()
            else:
                msf_f += lettre.upper()"""

        else:
            msf_f += x
    
    print(msf_f)
