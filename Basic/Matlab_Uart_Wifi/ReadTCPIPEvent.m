function ReadTCPIPEvent(t,~)
% Get TCPIP Data
Data = fscanf(t);
disp("TCPIP: ");
disp(Data);
end

