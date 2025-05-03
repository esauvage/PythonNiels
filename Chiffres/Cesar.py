def encrypt(texte, cle):
    resultat = ""

    for char in texte:
        if char.islower():
            resultat += chr((ord(char) - ord('a') + cle) % 26 + ord('a'))

        elif char.isupper():
            resultat += chr((ord(char) - ord('A') + cle) % 26 + ord('A'))

        else:
            resultat += char

    return resultat

def decrypt(texte, cle):
    return encrypt(texte, -cle)

if __name__ == "__main__":
    texte = input("Entrez votre message : ")
    cle = int(input("Entrez votre clé : "))

    crypte = encrypt(texte, cle)
    print("Chiffré :", crypte)

    decrypte = decrypt(crypte, cle)
    print("Déchiffré :", decrypte)
