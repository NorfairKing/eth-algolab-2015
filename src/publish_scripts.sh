txttopdf () {
  infile="$1"
  outfile="$2"
  enscript "$infile" --output=- | ps2pdf - > "$outfile"
}

txttopdf makefile makefile.pdf > /dev/null 2>&1
txttopdf algo algo.pdf         > /dev/null 2>&1

