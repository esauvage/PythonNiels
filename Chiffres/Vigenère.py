import base64
import argparse

parser = argparse.ArgumentParser()

parser.add_argument("chiffrer")
parser.add_argument("dechiffrer")
parser.add_argument("cle")

args = parser.parse_args()

def encrypt(texte, cle):
    resultat = ""

    for x, char in enumerate(texte):
        if char.islower():
            resultat += chr((ord(char) - ord('a') + ord(cle[x%len(cle)])) % 26 + ord('a'))
        elif char.isupper():
            resultat += chr((ord(char) - ord('A') + ord(cle[x%len(cle)])) % 26 + ord('A'))
        else:
            resultat += char

    return base64.b64encode(bytes(resultat, "utf-8")).decode("utf-8")

def decrypt(texte, cle):
    resultat = ""
    texte = base64.b64decode(bytes(texte, "utf-8")).decode("utf-8")

    for x, char in enumerate(texte):
        if char.islower():
            resultat += chr((ord(char) - ord('a') - ord(cle[x%len(cle)])) % 26 + ord('a'))
        elif char.isupper():
            resultat += chr((ord(char) - ord('A') - ord(cle[x%len(cle)])) % 26 + ord('A'))
        else:
            resultat += char

    return resultat

if __name__ == "__main__":
    if args.cle:
        if args.chiffrer:
            print("Chiffré : ", encrypt(args.chiffrer, args.cle))
            exit()

        elif args.dechiffrer:
            print("Déchiffré : ", decrypt(args.dechiffrer, args.cle))
            exit()

    texte = input("Entrez votre texte : ")
    cle = input("Entrez votre clé : ")

    while True:
        choix = input("Chiffrer ou déchiffrer ? ")

        if "déchiffrer".lower() in choix or "dechiffrer".lower() in choix:
            decrypte = decrypt(texte, cle)
            print("Déchiffré :", decrypte)
            exit()

        elif "chiffrer".lower() in choix:
            crypte = encrypt(texte, cle)
            print("Chiffré :", crypte)
            exit()

        else:
            print("Entrez 'chiffrer' ou 'déchiffrer' : ")
