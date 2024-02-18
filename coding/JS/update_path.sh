sed -i '' '/mobile1/d' *.html

replace1=$(sed -n 1p edit1.txt)
sed -i '' ' /homework1/ s|<.*>|'"${replace1}"'| ' *.html
# echo "${replace1}"

replace2=$(sed -n 2p edit1.txt)
sed -i '' ' /og:url/ s|<.*>|'"${replace2}"'| ' *.html
# echo "${replace2}"

gawk -i inplace ' {gsub(/XXXXX.html/, FILENAME)}1 ' *.html

replace3=$(sed -n 3p edit1.txt)
replace4=$(sed -n 4p edit1.txt)
sed -i '' ' /<li.*>/ s|'"${replace3}"'|'"${replace4}"'| ' *.html
