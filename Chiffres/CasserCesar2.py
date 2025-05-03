# Fonction testant les 25 possibilités possibles pour la clé.
def casser(texte):
    for cle in range(25):
        resultat = ""

        for char in texte:
            if char.islower():
                resultat += chr((ord(char) - ord('a') + cle) % 26 + ord('a'))
            elif char.isupper():
                resultat += chr((ord(char) - ord('A') + cle) % 26 + ord('A'))
            else:
                resultat += char

        solutions.append(resultat)

# Fonction ouvrant un dictionnaire de mots français et retirant le saut de ligne.
def liste_français(liste):
    with open(liste, "r") as f:
        f = f.readlines()

    # Crée une autre liste ne contenant pas contenant le saut de ligne.
    for x in f:
        dico.append(x.strip())

# Focntion vérifiant le nombre d'apparitions de mots de la liste 'dico' dans chaque essais de la liste solution.
def mot_probable():
    for t in solutions:
        mot = 0

        # Décomposition en mots
        t2 = t.split(" ")

        for m in t2:
            # Vérification de chaque mot dans 'dico'.
            if m in dico:
                mot += 1

        phrase_mots[t] = mot
        mots_phrase[mot] = t

if __name__ == "__main__":
    solutions = []
    mots = []

    phrase_mots = {}
    mots_phrase = {}

    dico = []

    liste = "liste_francais.txt"

    texte = input("Entrez votre message : ")

    casser(texte)
    liste_français(liste)
    mot_probable()

    # On met dans la liste 'mots' tous les nombres d'apparitions de mots en commun avec la liste de mots français (Je sais c'est pas très clair).
    for h in phrase_mots:
        mots.append(phrase_mots[h])

    # On la trie par odre croissant.
    mots.sort()

    # On récupère le dernier nombre de la liste, qui est forcément le plus grand.
    dernier = mots[len(mots)-1]

    # Et on affiche la phrase auquel il appartient.
    print("Résultat le plus probable : " + mots_phrase[dernier])
