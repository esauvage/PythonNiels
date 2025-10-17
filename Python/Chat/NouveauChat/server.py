import socket
import threading
from datetime import datetime
import XOR
import time

IP = "localhost"
PORT = 7894
CLE = "Smurtz123"

serveur = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serveur.bind((IP, PORT))
serveur.listen(10)

clients = []
pseudos = []


def diffuser(message):
    for client in clients:
        try:
            client.send(message)
        except:
            pass


def gestionClient(client, pseudo):
    while True:
        try:
            # réception et déchiffrement du message
            data = client.recv(1024)
            if not data:
                break

            message = XOR.dechiffrer(data.decode("utf-8"), CLE)
            temps = datetime.now().strftime("%H:%M")

            # ~ with open("journal.txt", "a") as f:
            # ~ f.write(f"{message}\n")

            # Pour fabriquer l'historique des conversations
            f = open("journal.txt", "a")

            if message == "exit":
                index = clients.index(client)
                clients.remove(client)
                client.close()
                pseudo_quit = pseudos[index]
                pseudos.remove(pseudo_quit)

                f.write(f"{temps} <{pseudo_quit}> a quitté le chat\n")

                sortie = XOR.chiffrer(f"{temps} <{pseudo_quit}> a quitté le chat", CLE)
                diffuser(sortie.encode("utf-8"))
                break

            else:
                f.write(f"{temps} <{pseudo}> {message}\n")

                sortie = XOR.chiffrer(f"{temps} <{pseudo}> {message}", CLE)
                diffuser(sortie.encode("utf-8"))

            f.close()

        except Exception as e:
            print("Erreur client:", e)

            index = clients.index(client)
            clients.remove(client)
            client.close()
            pseudo_quit = pseudos[index]
            pseudos.remove(pseudo_quit)

            break


def gestionConnexion():
    while True:
        client, adresse = serveur.accept()
        print(f"Connexion établie avec {str(adresse)}")

        pseudo = client.recv(1024).decode("utf-8")
        clients.append(client)
        pseudos.append(pseudo)

        temps = datetime.now().strftime("%H:%M")

        print(f"{temps} <{pseudo}> a rejoint le chat")

        message_bienvenue = XOR.chiffrer("Bienvenue sur Tchatooine !", CLE)
        client.send(message_bienvenue.encode("utf-8"))

        f = open("journal.txt", "r+")
        lignes = f.readlines()

        if len(lignes) > 30:
            lignes = [""]

        f.close()

        for ligne in lignes:
            client.send(XOR.chiffrer(ligne.strip(), CLE).encode("utf-8"))
            time.sleep(0.01)

        f = open("journal.txt", "a")
        f.write(f"{temps} <{pseudo}> a rejoint le chat\n")
        f.close()

        # ~ message_bienvenue = XOR.chiffrer("Bienvenue sur Tchatooine !", CLE)
        # ~ client.send(message_bienvenue.encode("utf-8"))

        # ~ time.sleep(0.01)

        msg_entree = XOR.chiffrer(f"{temps} <{pseudo}> a rejoint le chat", CLE)
        diffuser(msg_entree.encode("utf-8"))

        threadClient = threading.Thread(target=gestionClient, args=(client, pseudo))
        threadClient.start()


if __name__ == "__main__":
    print("Serveur Tchatooine en marche sur le port", PORT)
    gestionConnexion()
