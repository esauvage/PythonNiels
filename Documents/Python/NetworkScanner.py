import socket
import ipaddress

def scan_host(ip, ports=[22, 80, 443, 8080, 3306]):
    """Scan un hôte pour détecter les ports ouverts."""
    print(f"\nScan de {ip}...")
    for port in ports:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(1)  # Temps limite pour la connexion
        result = s.connect_ex((ip, port))
        if result == 0:
            print(f"    Port {port} ouvert")
        s.close()

def scan_network(network):
    """Scan une plage d'IP pour trouver les hôtes actifs."""
    print(f"\nScan du réseau {network}...")
    for ip in ipaddress.IPv4Network(network, strict=False):
        try:
            socket.gethostbyaddr(str(ip))
            scan_host(str(ip))
        except socket.herror:
            pass  # IP inactive

# Exécution du scan
network = input("Entrez la plage réseau (ex: 192.168.1.0/24) : ")
scan_network(network)

