#!/bin/bash

REPO_PATH="./repo_config"
CONFIG_PATH="./my_config"

if [ ! -f "$REPO_PATH" ]; then
    echo "no folder to work from, couldn't find repo configurations $REPO_PATH"
    exit 1
fi

if [ ! -f $CONFIG_PATH ]; then
    echo "$CONFIG_PATH doesn't exist creating a new one"
    touch "$CONFIG_PATH"
fi

repo_hosts=$(grep $REPO_PATH -e '^Host' | awk '{print $2}')

echo "$REPO_PATH: going to iterate on hosts"
for host in $repo_hosts; do
    echo "================="
    ip=$(grep $REPO_PATH -e $host -A 1 | grep HostName | awk '{print $2}')
    echo "host=$host ip=$ip"

    config_host=$(grep $CONFIG_PATH -e $host | awk '{print $2}')
    if [[ -z $config_host ]]; then
        echo "adding entry to "$host" config file"
        echo "$(grep $REPO_PATH -e $host -A 1)" >> $CONFIG_PATH
        echo "" >> $CONFIG_PATH
        continue
    fi

    config_result=$(grep -n $CONFIG_PATH -e $host -A 1 | grep HostName)
    line_number=$(echo $config_result | grep -o '[0-9]\+' | head -n 1)
    config_ip=$(echo $config_result | awk '{print $3}')

    echo "config host=$host ip=$config_ip"

    if [ "$ip" == "$config_ip" ]; then
        echo "$host is updated with ip $ip"
        continue
    fi

    echo "replacing $config_ip in $ip in my_config on line $line_number"
    sed -i "${line_number}s/${config_ip}/${ip}/" $CONFIG_PATH
done