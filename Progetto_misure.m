% Data loading
close all
clear all
clc

[file path] = uigetfile('datiserial.csv');

cd(path)
data = load(file);
adc1 = []; adc2 = []; millis = []; time = []; 
millis = data(:,1) - data(1,1);
time = millis/1000;
adc1 = data(:,2); % [V]
adc1=adc1(6:3978);
time=time(6:3978);
sampling = 1./mean(diff(time));% [Hz]

% Filtro passa basso
cutoff_freq = 2; % Frequenza di taglio in Hz
order = 1; % Ordine del filtro
Wn = cutoff_freq / (0.5 * sampling); % Frequenza normalizzata
[B, A] = butter(order, Wn, 'low');
signal_filtered = filtfilt(B, A, adc1);

% Trova i picchi nel segnale filtrato
figure
[pks, locs] = findpeaks(signal_filtered ,'MinPeakHeight', 1, 'MinPeakDistance', 50);
plot( time, signal_filtered, time(locs), pks, 'r*')
n_respiri = length(locs)
xlabel('Time [s]', 'Interpreter','latex')
ylabel('V{output} [V]', 'Interpreter','latex')
title('Respiri nel tempo', 'Interpreter', 'latex', 'FontSize', 24)
