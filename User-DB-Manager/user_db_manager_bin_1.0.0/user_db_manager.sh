#! /bin/sh -e
### BEGIN INIT INFO
# Provides:          user_db_manager.sh
# Required-Start:    $local_fs $network
# Required-Stop:     $local_fs
# Default-Start:     2 3 4 5
# Default-Stop:      0
# Short-Description: user_db_manager.sh
### END INIT INFO


PATH=/bin:/usr/local/bin
Killall=/usr/bin/killall

case "$1" in
    start)
	echo "pdf_gen :start Running user_db_manager!!!!!" >&2
        /usr/local/bin/user_db_manager &
	sleep 1
	echo sudo 600 > /proc/sys/net/ipv4/tcp_keepalive_time
	echo sudo 60 > /proc/sys/net/ipv4/tcp_keepalive_intvl
	echo sudo 20 > /proc/sys/net/ipv4/tcp_keepalive_probes
	;;
		
    restart|reload|force-reload)
        echo "pdf_gen :Restart Running user_db_manager!!!!!" >&2
	/usr/local/bin/user_db_manager &
	sleep 1
	echo sudo 600 > /proc/sys/net/ipv4/tcp_keepalive_time
	echo sudo 60 > /proc/sys/net/ipv4/tcp_keepalive_intvl
	echo sudo 20 > /proc/sys/net/ipv4/tcp_keepalive_probes
        ;;
    stop)
        echo "pdf_gen : Killing exemain!!!!!" >&2
        $Killall -9 user_db_manager
        ;;
esac
