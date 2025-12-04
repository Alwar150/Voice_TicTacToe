server = startTcpServer();
function tserver = startTcpServer()
    % Define la dirección local (localhost) y el puerto
    serverAddress = '192.168.56.1';
    serverPort = 5000;

    disp('--------------------------------------------------');
    disp(['Iniciando servidor TCP en ' serverAddress ':' num2str(serverPort) '...']);

    % Crea el objeto tcpserver
    % Usamos la sintaxis Name,Value para configurar los Callbacks durante la creación.
    tserver = tcpserver(serverAddress, serverPort, ...
                        "ConnectionChangedFcn", @handleConnectionChange); % Usamos terminador para mensajes claros
    
    % OPCIONAL: Si esperas cadenas de texto (como en C++), ajusta el terminador
    configureTerminator(tserver,"LF"); % Usa 'LF' (Line Feed) o 'CR/LF' (Windows)
    configureCallback(tserver,"terminator",@handleDataAvailable);
    disp('Servidor iniciado. Esperando conexiones (Presiona Ctrl+C o usa clear tserver para detener).');
    disp('--------------------------------------------------');
end


% --- DEFINICIÓN DE FUNCIONES DE CALLBACK ---

% Callback que se ejecuta cuando un cliente se conecta o desconecta.
% Es invocado por la propiedad 'ConnectionChangedFcn'.
function handleConnectionChange(src, ~)
    % src es el objeto tcpserver (el servidor)
    
    if src.Connected
        % --- EVENTO DE CONEXIÓN ---
        disp(['✅ Cliente conectado: ' src.ClientAddress ' en el puerto ' num2str(src.ClientPort)]);
        % Limpia cualquier dato antiguo que pueda estar en el buffer
        flush(src); 
    else
        % --- EVENTO DE DESCONEXIÓN ---
        disp(['❌ Cliente desconectado: ' src.ClientAddress]);
    end
end

% Callback que se ejecuta cuando hay datos disponibles para leer,
% desencadenado por el terminador ('\n' si Terminator='LF').
% Es invocado por la propiedad 'BytesAvailableFcn'.
function handleDataAvailable(src, ~)
    % Leemos la cadena de texto hasta el terminador
    data = readline(src);
    
    disp(['<< RECIBIDO de ' src.ClientAddress ': ' data]);
    
    % OPCIONAL: Responder al cliente
    response = ['DONE'];
    writeline(src, response);
    disp(['>> ENVIADO a ' src.ClientAddress ': ' response]);

    % Asegúrate de que el buffer esté limpio si esperas más datos
    flush(src); 
end