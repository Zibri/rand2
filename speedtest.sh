if [ -z "$DEVSHELL_IP_ADDRESS" ]
then
sudo nice -n -20 ./rand2 1 1| pv -ptebaSs 800G >/dev/null
else
sudo nice -n -20 ./rand2 1 1| pv -ptebaSs 80G >/dev/null
fi
