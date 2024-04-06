clf
hold on
load('filtered.txt', "-ascii")
load('signal.txt', "-ascii")

signal=transpose(signal)
filtered=transpose(filtered)

indicies=1:1:length(signal)

plot(indicies, signal)
plot(indicies, filtered)

gird

hold off
