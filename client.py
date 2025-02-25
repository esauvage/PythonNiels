import socket
import threading
from tkinter import *
from tkinter import scrolledtext

fenêtre = Tk()
fenêtre.title("Chat - Client")
fenêtre.geometry("720x480")
fenêtre.resizable(0, 0)

IP = "92.151.59.90"
PORT = 5555

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((IP, PORT))

pseudo = input("Entrez votre pseudo : ")
client.send(bytes(pseudo, "utf-8"))

chat_box = scrolledtext.ScrolledText(fenêtre, width=80, height=20)
chat_box.pack(pady=20)

message_entry = Entry(fenêtre, width=50)
message_entry.pack(pady=10)

def envoyer_message():
    message = message_entry.get()
    if message:
        client.send(bytes(message, "utf-8"))
        message_entry.delete(0, END)

send_button = Button(fenêtre, text="Envoyer", command=envoyer_message)
send_button.pack(pady=5)

def recevoir_message():
    while True:
        try:
            message = client.recv(1024).decode("utf-8")
            chat_box.insert(END, message + "\n")
            chat_box.yview(END)
        except:
            break

thread_reception = threading.Thread(target=recevoir_message)
thread_reception.start()

fenêtre.mainloop()
