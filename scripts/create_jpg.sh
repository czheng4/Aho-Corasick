jgraph -P scripts/plot_jgraph.jgr | ps2pdf - | convert -density 300 - -quality 100 simp1.jpg
convert simp1.jpg -gravity South -chop 0x1200 -gravity North -chop 0x1200 graph.jpg
