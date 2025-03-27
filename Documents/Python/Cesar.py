import string

caracs = string.ascii_letters
msg = input("Entrez message : ")
cle = input("Entrez votre clé : ")
msf_f = ""

for x in msg:
    if x == " ":
        msf_f += " "

    elif x in caracs:
        index = caracs.index(x)

        new_index = (index + cle) % len(caracs)
        lettre = caracs[new_index]
        msf_f += lettre

    else:
        msf_f += x

print(msf_f)