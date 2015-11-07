pdfs=""
for i in *; do
  if [ -d $i ] ; then
    cd $i

    if [ -f writeup.tex ] ; then
      ../algo publish > /tmp/publish.out 2>&1
      if [[ "$?" == "0" ]] ; then
        pdfs="$pdfs $i/writeup.pdf"
      else
        cat /tmp/publish.out
      fi
    else
      echo "writeup for $i missing"
    fi

    cd ..
  fi
done

txttopdf () {
  infile="$1"
  outfile="$2"
  enscript "$infile" --output=- | ps2pdf - > "$outfile"
}

txttopdf makefile makefile.pdf
txttopdf algo algo.pdf

echo $pdfs

RESULT="algolab-writeups-2015.pdf"

cmd="pdfunite $pdfs algo.pdf makefile.pdf $RESULT"
echo $cmd
$cmd

