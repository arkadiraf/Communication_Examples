function ReadSerialEvent(s,~)
% get serial data
MSG_Serial = readline(s);
disp("Serial: ");
disp(MSG_Serial);
end

