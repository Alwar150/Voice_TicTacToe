% Crear servidor escuchando en cualquier dirección y puerto 5000
server = tcpserver('192.168.1.134',5000, "Timeout", 30);

% Configurar callback para cuando llegue una línea terminada (por ejemplo '\n')
server.configureCallback("terminator", @onClientMessage);

function onClientMessage(src, ~)
    msg = readline(src);
    fprintf("Recibido: %s\n", msg);
    % Aquí parseas el mensaje y ejecutas la lógica
end
