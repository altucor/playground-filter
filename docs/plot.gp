# Tell gnuplot the file uses comma separators
set datafile separator ","

# Labels
set xlabel "Time (s)"
set ylabel "Value"
set title  "Raw vs Filtered signal"
set key top right

# Plot col 1 (t) vs col 2 (raw) AND col 1 vs col 3 (filtered)
# 'skip 1' skips the header row
plot filename every ::1 using 1:2 with linespoints title "raw", \
     filename every ::1 using 1:3 with lines     title "filtered"