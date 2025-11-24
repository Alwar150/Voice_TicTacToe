try
    % --- Configuration ---
    serverIP   = "127.0.0.1";  % Change to your server's IP address
    serverPort = 5000;        % Change to your server's port number
    
    % --- Create TCP client ---
    % Timeout is in seconds; increase if server is slow to respond
    client = tcpclient(serverIP, serverPort, "Timeout", 5);
    fprintf("Connected to %s:%d\n", serverIP, serverPort);
    
    % --- Send data ---
    message = "Hello from MATLAB!";
    writeline(client, message);
    fprintf("Sent: %s\n", message);
    
    % --- Wait briefly for server to respond ---
    pause(0.5); % Adjust as needed for server response time
    
    % --- Read available data ---
    if client.NumBytesAvailable > 0
        data = readline(client);
        response = char(data);
        fprintf("Received: %s\n", response);
    else
        fprintf("No data received from server.\n");
    end
    
    % --- Clean up ---
    clear client; % Automatically closes connection
    fprintf("Connection closed.\n");

catch ME
    % Error handling
    fprintf("Error: %s\n", ME.message);
end
