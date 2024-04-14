clf
hold on
load('output.txt', "-ascii")

output=transpose(output)

indicies=1:1:length(output)

plot(indicies, output)
grid on

hold off
