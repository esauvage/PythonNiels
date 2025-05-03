import hashlib

texte = input("Entrez la chaine de caractère à hasher : ")

print(hashlib.sha384(bytes(texte, "utf-8")).hexdigest())
