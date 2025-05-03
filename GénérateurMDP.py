import random
import string

carac = string.ascii_letters + string.digits + string.punctuation

if __name__ == "__main__":
    mdp = ""

    longueur = input("Entrez la longueur du mot de passe voulu : ")

    try:
        for x in range(int(longueur)):
            mdp += random.choice(carac)

    except:
        print("Format non-valide.")
        exit()

    print("Mot de passe généré : " + mdp)
