#
# Regular cron jobs for the qontacts package
#
0 4	* * *	root	[ -x /usr/bin/qontacts_maintenance ] && /usr/bin/qontacts_maintenance
