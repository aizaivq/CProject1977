export count=0;
while true;
do
	echo "while count $count"
	sleep 1
	if [ $count -ge 10 ];then
		break
	fi
	let count+=1
done
