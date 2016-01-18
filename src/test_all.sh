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

nb_dirs=$(ls -1 -d */ | wc -l)
counter="1"

dirs=$(ls -1 -d */)
for i in $dirs; do
  i="${i::-1}" # drop last char of i

  cd $i

  echo -n "["
  printf "%2s" $counter
  echo -n "/$nb_dirs] "

  is=$(printf "%20s" $i)
  echo -n "$is"
  s=$(date +%s.%N)
  ../algo test >/dev/null 2>&1
  ex="$?"
  e=$(date +%s.%N)
  diff=$(echo "($e - $s) * 1000" | bc)
  t=$(printf " %10.2f ms\n" "$diff")
  if [[ "$ex" == "0" ]] ; then
    print_colored_text GREEN "$t"
  else
    print_colored_text RED "$t"
  fi
  echo

  cd ..

  counter=$(($counter + 1))
done

