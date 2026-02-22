#!/bin/bash

APP1_PID=""
APP2_PID=""
export bin_dir="build"
export fifo_path="/tmp/app_fifo"

start_apps() {
    echo "Starting App2..."
    if [ -d "$bin_dir" ]; then
        cd "./$bin_dir"
        if [ ! "$(pidof app2)" ]; then    
            ./app2 &
            APP2_PID=$!
        else
            echo "App2 have already started"
        fi
    else
        echo "Build dir/App2 doesn't exist"
        exit 1
    fi

    echo "Starting App1..."
    if [ -f "app1" ]; then
        if [ "$(pidof app1)" ]; then
          kill $(pidof app1)
        fi
        ./app1 
    else
        echo "App1 doesn't exist, stopping app2"
        kill $APP2_PID
        if [ -f /tmp/app_fifo ]; then
            rm /tmp/app_fifo
        fi
        exit -1
    fi
}

stop_apps() {
    echo "Stopping applications..."

    if  [[ ! "$(pidof app2)" &&  ! "$(pidof app1)" ]]; then
        echo "Apps aren't active"
    else

        if [ "$(pidof app2)" ]; then
            kill $(pidof app2)
            echo "App2 are stopped"
        fi

        if [ "$(pidof app1)" ]; then
            kill $(pidof app1)
            echo "App1 are stopped"
        fi

        echo "Apps are stopped."
    fi

    if [ -f /tmp/app_fifo ]; then
        echo "kill fifo"
        rm /tmp/app_fifo
    fi

}

case "$1" in
    start)
        start_apps
        ;;
    stop)
        stop_apps
        ;;
    restart)
        stop_apps
        sleep 1
        start_apps
        ;;
    *)
        echo "Usage: $0 {start|stop|restart}"
        exit 1
        ;;
esac