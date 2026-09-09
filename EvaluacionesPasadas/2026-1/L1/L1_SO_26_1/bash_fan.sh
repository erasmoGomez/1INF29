while true; do
    clear
    ps -l | head -1
    ps -l | grep '[f]an'
    sleep 0.8
done