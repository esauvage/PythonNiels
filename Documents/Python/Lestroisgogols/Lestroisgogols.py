import getpass
import hashlib

objet_hash = hashlib.sha256()

bon_mdp = "b44adba65a265d07673d839bd45a432c23f44f140ed190b4c2daff0fe98a5845"

tentatives = 3

# Les humains sont des patates.
mdp = getpass.getpass("Entrez le mot de passe : ")

while tentatives > 0:
    objet_hash = hashlib.sha256()
    objet_hash.update(mdp.encode())
    mdp_hash = objet_hash.hexdigest()

    if mdp_hash == bon_mdp:
        with open("/home/nsauvage/Documents/Python/Lestroisgogols/Logo.txt", "r") as logo:
            logo_content = logo.read()

        print(logo_content)
        print("Bienvenue dans le centre de commandement des Trois Gogols.")
        break
    else:
        tentatives -= 1
        if tentatives == 0:
            print("Trop de mots de passe incorrects.")
            exit()
        else:
            mdp = getpass.getpass("Mot de passe incorrect, réessayez : ")

indentifiant = input("Identifiez-vous : ")

if indentifiant == "Fred":

    tentatives = 3

    bon_mdp = "401f1721a42a814961323c460dd7d2036231ddf590b5d898c9cd086a46526bdb"

    while tentatives > 0:
        mdp = getpass.getpass("Entrez votre mot de passe : ")

        objet_hash = hashlib.sha256()
        objet_hash.update(mdp.encode())
        mdp_hash = objet_hash.hexdigest()

        if mdp_hash == bon_mdp:
            print("Bienvenue, Chef. Vous avez les droits de commandements.")
            print("Pour éditer une mission, ouvrez un éditeur de texte et détailler votre mission.")

            choix = input("Souhaitez-vous créer une nouvelle mission ? (o/n) : ")

            if choix == "o":
                fichier = input("Pour ajouter la mission à la liste des missions, entrez le chemin complet du fichier : ")

                try:
                    with open("MSSIONS", "w") as f:
                        with open(fichier, "r") as f2:
                            contenu = f2.readlines()

                    f.write(contenu + "\n")

                except:
                    print("Votre chemin n'existe pas.")
                    exit()
                
                print()
                print("="*20 + "MISSION" + "="*20)

                with open("/home/nsauvage/Documents/Python/Lestroisgogols/Missions.txt", "a") as fichier2:
                    fichier2.write(fichier)

                with open(fichier, "r") as fichier:
                    fichier = fichier.read()

                print(fichier)
            
            else:
                pass

            break

        else:
            print("Mot de passe incorrect.")

elif indentifiant == "George":

    tentatives = 3

    bon_mdp = "3d28271ec52e3d07fe14f5f16d01f2c09cbcac1949f9904b305136d0edbee12d"

    while tentatives > 0:
        mdp = getpass.getpass("Entrez votre mot de passe : ")

        objet_hash = hashlib.sha256()
        objet_hash.update(mdp.encode())
        mdp_hash = objet_hash.hexdigest()

        if mdp_hash == bon_mdp:
            print("Bienvenue, George. Vous avez les droits d'administrateur.")
            break

        else:
            print("Mot de passe incorrect.")

elif indentifiant == "Jean-Michel":

    tentatives = 3

    bon_mdp = "3fa67f66cdf8cfb3e36ee91fe4f3ea45cd8308cc49ffe4c9c23c7a71ced7435c"

    while tentatives > 0:
        mdp = getpass.getpass("Entrez votre mot de passe : ")

        objet_hash = hashlib.sha256()
        objet_hash.update(mdp.encode())
        mdp_hash = objet_hash.hexdigest()

        if mdp_hash == bon_mdp:
            print("Bienvenue, Jean-Michel. Vous avez les droits de base.")
            break

        else:
            objet_hash = hashlib.sha256()
            objet_hash.update(mdp.encode())
            mdp_hash = objet_hash.hexdigest()

            tentatives -= 1
            if tentatives == 0:
                print("Trop de mots de passe incorrects.")
                exit()
            else:
                mdp = getpass.getpass("Mot de passe incorrect, réessayez : ")

else:
    print("Identifiant inconnu.")
