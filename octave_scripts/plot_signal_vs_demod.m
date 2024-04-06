clf
hold on
load('demod.txt', "-ascii")
load('amsignal.txt', "-ascii")

signal=transpose(signal)
demod=transpose(demod)

indicies=1:1:length(signal)

plot(indicies, signal)
plot(demod(2,:), demod(1,:))

hold off
