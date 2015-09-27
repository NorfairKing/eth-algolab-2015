pdfs=""

for i in *; do
  if [ -d $i ] ; then
    cd $i
  
    ../algo publish
    pdfs="$pdfs $i/writeup.pdf"

    cd ..
  fi
done

echo $pdfs

RESULT="algolab-writeups-2015.pdf"

cmd="pdfunite $pdfs $RESULT"
echo $cmd
$cmd
