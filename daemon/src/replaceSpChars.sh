#!/bin/bash

for file in "$@"
do
   sed -i 's/&lt;/</g' $file
   sed -i 's/&gt;/>/g' $file
   sed -i 's/&quot;/"/g' $file
done
