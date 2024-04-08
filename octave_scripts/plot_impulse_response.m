clf
hold on
load('filtered_impulse.txt', "-ascii")
load('impulse.txt', "-ascii")

impulse=transpose(impulse)
filtered_impulse=transpose(filtered_impulse)

indicies=1:1:length(impulse)

plot(indicies, impulse)
plot(indicies, filtered_impulse)

gird
        hold off

