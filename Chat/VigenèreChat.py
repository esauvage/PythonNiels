def encrypt(texte, cle):
    resultat = ""

    for x, char in enumerate(texte):
        if char.islower():
            resultat += chr((ord(char) - ord('a') + ord(cle[x%len(cle)])) % 26 + ord('a'))

        elif char.isupper():
            resultat += chr((ord(char) - ord('A') + ord(cle[x%len(cle)])) % 26 + ord('A'))

        else:
            resultat += char

    return resultat

def decrypt(texte, cle):
    resultat = ""

    for x, char in enumerate(texte):
        if char.islower():
            resultat += chr((ord(char) - ord('a') - ord(cle[x%len(cle)])) % 26 + ord('a'))

        elif char.isupper():
            resultat += chr((ord(char) - ord('A') - ord(cle[x%len(cle)])) % 26 + ord('A'))

        else:
            resultat += char

    return resultat
