pdfs=""

for i in *; do
  if [ -d $i ] ; then
    cd $i

    ../algo test
  
    if [ -f writeup.tex ] ; then
      ../algo publish
      pdfs="$pdfs $i/writeup.pdf"
    fi

    cd ..
  fi
done

echo $pdfs

RESULT="algolab-writeups-2015.pdf"

cmd="pdfunite $pdfs $RESULT"
echo $cmd
$cmd
