% TCP/IP telnet example with M5Stack , Matlab 2019b
% Arkadi Rafalovich , Arkadiraf@gmail.com , 17/02/2020                                     
% https://www.mathworks.com/help/instrument/tcpip.html
% https://www.mathworks.com/help/matlab/ref/serialport.html
% https://www.mathworks.com/help/matlab/creating_plots/making-graphs-responsive-with-data-linking.html
clc
clear
%% Init serial connection
s = serialport("COM3",115200);                          % Enter comport settings
configureTerminator(s,"CR/LF");                         % set Terminator
configureCallback(s,"terminator",@ReadSerialEvent);     % Attach callback function

%% Init TCPIP connection
t = tcpip('192.168.10.1', 23);          % enter your device ip address , Server port
t.Terminator = 'CR/LF';                 % Terminator
%t.BytesAvailableFcnMode = 'byte';      % callback function settings, default : 'terminator'
t.BytesAvailableFcn = @ReadTCPIPEvent;  % attach callback function
% Init connection
fopen(t);

%% Send messages
% TCPIP
fprintf(t,'Hello World');

% Serial
writeline(s,"Hello World");

%% close connection TCPIP
% Close connection TCPIP
fclose(t);
delete(t);
clear('t');
% close connection Serial callback
configureCallback(s,"off");
delete(s);
clear('s');

%% find availbale objects if a proper disconnection wasn`t performed.
instrfind
delete(instrfind)
