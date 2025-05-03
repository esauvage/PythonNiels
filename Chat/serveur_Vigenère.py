import socket
import threading
import VigenèreChat

IP = "0.0.0.0"
PORT = 5555

CLE = """zaP$RGb/u;Xu#TBZ~KN>,:R9ll4t@K<^ncQp@J9w-&a8DISQ9.5~2DVG^z@)D~PniPt6KtMtY7^A^d5ofuz@W6&w_ik={bR)U!*4"""

serveur = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

serveur.bind((IP, PORT))

serveur.listen(2)

clients = []
pseudos = []

def diffuser(message):
    for client in clients:
        client.send(bytes(message, "utf-8"))

    message = VigenèreChat.decrypt(message, CLE)

    with open("journal.txt", "a") as f:
        f.write(message + "\n")


def gestion_connexion():
    while True:
        client, adresse = serveur.accept()
        print(f"Connexion établie avec {str(adresse)}")

        pseudo = client.recv(1024).decode("utf-8")
        if len(pseudo) > 32:
            continue

        clients.append(client)
        pseudos.append(pseudo)

        print(f"{pseudo} a rejoint le chat")

        f = open("journal.txt", "r")
        a_envoyer = f.readlines()

        f.close()

        for l in range(len(a_envoyer)):
            envoyer = a_envoyer[l]

            envoyer = VigenèreChat.encrypt(envoyer, CLE)
            client.send(bytes(envoyer, "utf-8"))

        thread_client = threading.Thread(target=gestion_client, args=(client, pseudo))
        thread_client.start()

def gestion_client(client, pseudo):
    while True:
        try:
            message = client.recv(1024).decode("utf-8")

            if message == "exit":
                index = clients.index(client)
                clients.remove(client)
                client.close()

                pseudo = pseudos[index]
                pseudos.remove(pseudo)

                a_envoyer = VigenèreChat.encrypt(f"{pseudo} a quitte le chat", CLE)

                diffuser(a_envoyer)
                break

            else:
                diffuser(message)

        except:
            index = clients.index(client)
            clients.remove(client)
            client.close()

            pseudo = pseudos[index]
            pseudos.remove(pseudo)

            a_envoyer = VigenèreChat.encrypt(f"{pseudo} a quitte le chat", CLE)

            diffuser(a_envoyer)
            break

print("Le serveur de chat fonctionne.")
gestion_connexion()
