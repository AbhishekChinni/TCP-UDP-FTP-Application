#!/bin/bash
for i in $(ls|grep $1)
do
	ls -lah $i|awk '{print $9" "$5}' >> final
done

