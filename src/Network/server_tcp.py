import socket

HOST = "0.0.0.0"   # Escuchar en todas las interfaces
PORT = 5000        # Debe coincidir con el puerto que usa tu Qt (p. ej. 5000)

def main():
    print(f"Servidor TCP escuchando en {HOST}:{PORT}")
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
        server.bind((HOST, PORT))
        server.listen(1)  # Solo una conexión a la vez
        print("Esperando conexión...")

        conn, addr = server.accept()
        print(f"Conectado desde {addr}")

        with conn:
            while True:
                data = conn.recv(1024)
                if not data:
                    print("Cliente desconectado.")
                    break

                mensaje = data.decode("utf-8").strip()
                print(f"📩 Recibido: {mensaje}")

                # Simular respuesta del robot
                respuesta = "OK: movimiento completado\n"
                conn.sendall(respuesta.encode("utf-8"))

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nServidor detenido.")
