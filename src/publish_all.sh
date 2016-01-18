ESC_SEQ="\x1b["
COL_RESET=$ESC_SEQ"39;49;00m"
COL_RED=$ESC_SEQ"31;11m"
COL_GREEN=$ESC_SEQ"32;11m"
COL_YELLOW=$ESC_SEQ"33;11m"
COL_BLUE=$ESC_SEQ"34;11m"
COL_MAGENTA=$ESC_SEQ"35;11m"
COL_CYAN=$ESC_SEQ"36;11m"
print_colored_text () {
  color=$1
  text=$2
  color_code="COL_$color"
  echo -e -n "${!color_code}$text$COL_RESET"
}


pdfs=""

nb_dirs=$(ls -1 -d */ | wc -l)
counter="1"

dirs=$(ls -1 -d */)
for i in $dirs; do
  i="${i::-1}" # drop last char of i

  cd $i

  echo -n "["
  printf "%2s" $counter
  echo -n "/$nb_dirs] "

  if [ -f writeup.tex ] ; then
    ../algo publish > /tmp/publish.out 2>&1
    if [[ "$?" == "0" ]] ; then
      pdfs="$pdfs $i/writeup.pdf"
      print_colored_text GREEN "$i"
    else
      cat /tmp/publish.out
      print_colored_text RED "$i"
    fi
  else
    print_colored_text RED "$i"
  fi
  echo

  cd ..

  counter=$(($counter + 1))
done

./publish_scripts.sh

RESULT="algolab-writeups-2015.pdf"

cmd="pdfunite $pdfs algo.pdf makefile.pdf $RESULT"
# echo $cmd
$cmd

