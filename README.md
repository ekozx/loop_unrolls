# loop_unrolls

<p>Compares the run time of unrolled vs regular loops.</p>
<p>Use "gcc -O0 prob_2.c" or "gcc -O0 prob_2_floats.c" for integers or floats respectively.</p>
<p>Note: the -O0 flag causes the compiler to run without optimizations, this is ideal to see the difference</p>

<h3> TODO: </h3>
<ul>
  <li>bash program to replace the for loops reiteration, I feel like that might be affecting performance.</li>
  <li>Actually explaining what's happening...</li>
</ul>

<h4>R code to generate a plot</h4>
<pre>
  integer_data = read.table("0_unrolls.txt")
  plot(integer_data$V1, xlab="for-loop iteration", ylab="seconds")
</pre>

<h4>Bash script to run once you've compiled your choice of c code:</h4>
<pre>./rerun.sh</pre>