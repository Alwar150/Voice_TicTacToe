% Cliente TCP
client = startTcpClient();
function tclient = startTcpClient()
    % Define la dirección del servidor y el puerto
    serverAddress = '192.168.56.1';
    serverPort = 5000;

    disp('--------------------------------------------------');
    disp(['Iniciando cliente TCP conectándose a ' serverAddress ':' num2str(serverPort) '...']);

    % Crea el objeto tcpclient
    tclient = tcpclient(serverAddress, serverPort);

    % Configura el terminador para mensajes claros
    configureTerminator(tclient, "CR/LF"); % Usa 'LF' (Line Feed) o 'CR/LF' (Windows)
    configureCallback(tclient,"terminator",@handleDataAvailable);
    % Conecta al servidor
    try 
        message= "Hi from Matalb";
        sendMessage(tclient, message);
    catch ME
        disp(['Error al conectar al servidor: ' ME.message]);
        return;
    end
end

% --- FUNCIONES DE CALLBACK ---

% Callback que se ejecuta cuando hay datos disponibles para leer
% desencadenado por el terminador ('\n' si Terminator='LF').
% Es invocado por la propiedad 'BytesAvailableFcn'.
function handleDataAvailable(src, ~)
    % Leemos la cadena de texto hasta el terminador
    data = readline(src);
    
    disp(['<< RECIBIDO del servidor: ' data]);
    
    % Asegúrate de que el buffer esté limpio si esperas más datos
    flush(src); 

    sendMessage(src, "1,1,X");
end

% Función para enviar mensajes al servidor
function sendMessage(client, message)
    try
        writeline(client, message);
        disp(['>> ENVIADO al servidor: ' message]);
    catch ME
        disp(['Error al enviar mensaje: ' ME.message]);
    end
end

% Ejemplo de uso:
% Para enviar un mensaje al servidor:
% sendMessage(client, '1,2,X');  % Ejemplo de coordenadas y carácter

% Para cerrar la conexión:
% disconnect(client);
% clear client;
